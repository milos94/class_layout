#include <benchmark/benchmark.h>
#include <vector>
#include <limits>
#include <cstdlib>
#include <compare>
#include <numeric>

struct good_data
{
    int value_;
    bool b1_, b2_;

    constexpr good_data(int val, bool b1, bool b2) noexcept
        : value_{val}, b1_{b1}, b2_{b2}
    {

    }    
    
    bool operator<(good_data const & rhs ) const noexcept
    {
        return value_ < rhs.value_ || b1_ < rhs.b1_ || b2_ < rhs.b2_;
    }
    
};

struct sinful_data
{
    bool b1_;
    int value_;
    bool b2_;

    constexpr sinful_data(int val, bool b1, bool b2) noexcept
        : b1_{b1}, value_{val}, b2_{b2}
    {

    }

    bool operator<(sinful_data const & rhs) const noexcept
    {
        return value_ < rhs.value_ || b1_ < rhs.b1_ || b2_ < rhs.b2_; 
    }

};


static void CustomArguments(benchmark::internal::Benchmark* b) {
  for (int base = 1; base < 1'000'000; base*=10)
  {
    for (int multiplier = 1; multiplier < 10; ++multiplier)
    {
        b->Arg(base*multiplier);
    }
  }
  b->Arg(10'000'000);
}

template<typename T>
auto generate_data(size_t lenght)
{
    static constexpr int half = std::numeric_limits<int>::max();
    std::srand(12345);

    std::vector<T> data{};
    data.reserve(lenght);

    for (size_t i = 0; i < lenght; ++i)
    {
        data.emplace_back(std::rand(), std::rand() < half, std::rand() > half);
    }

    return data;
}

static void generate_good_data(benchmark::State& state) {
  for (auto _ : state)
  {
    auto data = generate_data<good_data>(state.range(0));
  }
}

static void generate_sinful_data(benchmark::State& state) {
  for (auto _ : state)
  {
    auto data = generate_data<sinful_data>(state.range(0));
  }
}

auto for_each_lambda = [](auto ele) { ele.value_ += 10; ele.b1_ = false; };

static void for_each_good_data(benchmark::State& state)
{
    auto const data = generate_data<good_data>(state.range(0));
    for(auto _ : state)
    {
        state.PauseTiming();
        auto run_data = data;
        state.ResumeTiming();
        std::for_each(std::begin(run_data), std::end(run_data), for_each_lambda);
    }
}

static void for_each_sinful_data(benchmark::State& state)
{
    auto const data = generate_data<sinful_data>(state.range(0));
    for(auto _ : state)
    {
        state.PauseTiming();
        auto run_data = data;
        state.ResumeTiming();
        std::for_each(std::begin(run_data), std::end(run_data), for_each_lambda);
    }
}

static void sort_good_data(benchmark::State& state)
{
    auto const data = generate_data<good_data>(state.range(0));
    for(auto _ : state)
    {
        state.PauseTiming();
        auto run_data = data;
        state.ResumeTiming();
        std::sort(std::begin(run_data), std::end(run_data));
    }
}

static void sort_bad_data(benchmark::State& state)
{
    auto const data = generate_data<sinful_data>(state.range(0));
    for(auto _ : state)
    {
        state.PauseTiming();
        auto run_data = data;
        state.ResumeTiming();
        std::sort(std::begin(run_data), std::end(run_data));
    }
}

auto acc = [](int64_t lhs, auto const & rhs) -> int64_t
            { return lhs + rhs.value_;  };


static void accumulate_good_data(benchmark::State& state)
{
    auto const data = generate_data<good_data>(state.range(0));
    int64_t result{};
    for(auto _ : state)
    {
        result = 0;
        benchmark::DoNotOptimize(result = std::accumulate(std::cbegin(data), std::cend(data), 0, acc));
    }
}

static void accumulate_sinful_data(benchmark::State& state)
{
    auto const data = generate_data<sinful_data>(state.range(0));
    int64_t result{};
    for(auto _ : state)
    {
        result = 0;
        benchmark::DoNotOptimize(result = std::accumulate(std::cbegin(data), std::cend(data), 0, acc));
    }
}

BENCHMARK(generate_good_data)->Apply(CustomArguments)->Iterations(100);
BENCHMARK(generate_sinful_data)->Apply(CustomArguments)->Iterations(100);

BENCHMARK(for_each_good_data)->Apply(CustomArguments)->Iterations(100);
BENCHMARK(for_each_sinful_data)->Apply(CustomArguments)->Iterations(100);

BENCHMARK(sort_good_data)->Apply(CustomArguments)->Iterations(100);
BENCHMARK(sort_bad_data)->Apply(CustomArguments)->Iterations(100);

BENCHMARK(accumulate_good_data)->Apply(CustomArguments)->Iterations(100);
BENCHMARK(accumulate_sinful_data)->Apply(CustomArguments)->Iterations(100);

BENCHMARK_MAIN();