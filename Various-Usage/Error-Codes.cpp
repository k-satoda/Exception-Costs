#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <functional>
#include <cmath>
using namespace std;
using namespace std::chrono;

auto generate_test_data(int n)
{
   vector<int> v(n);
   generate_n(begin(v), n, [cur = -100]() mutable { return cur++; });
   random_device rd;
   mt19937 prng{ rd() };
   shuffle(begin(v), end(v), prng);
   return v;
}

bool process_or_signal(int n, int &result)
{
   if (n < 0) return false;
   result = n * 2;
   return true;
}

auto check_every_iteration(vector<int> v)
{
   clog.rdbuf(nullptr);
   auto before = high_resolution_clock::now();
   int nerrs = 0;
   for (auto & val : v)
   {
      int result;
      if (process_or_signal(val, result))
      {
         val = result;
      }
      else
      {
         ++nerrs;
      }
   }
   auto after = high_resolution_clock::now();
   clog << accumulate(begin(v), end(v), 0);
   return make_pair(after - before, nerrs);
}

auto check_general_early_exit(vector<int> v)
{
   clog.rdbuf(nullptr);
   sort(begin(v), end(v));
   auto before = high_resolution_clock::now();
   vector<int>::size_type i = 0;
   for (; i != v.size(); ++i)
   {
      int result;
      if (!process_or_signal(v[i], result)) break;
      v[i] = result;
   }
   auto after = high_resolution_clock::now();
   clog << accumulate(begin(v), end(v), 0);
   return make_pair(after - before, i);
}

auto check_general_late_exit(vector<int> v)
{
   clog.rdbuf(nullptr);
   sort(begin(v), end(v), greater<>{});
   auto before = high_resolution_clock::now();
   vector<int>::size_type i = 0;
   for (; i != v.size(); ++i)
   {
      int result;
      if (!process_or_signal(v[i], result)) break;
      v[i] = result;
   }
   auto after = high_resolution_clock::now();
   clog << accumulate(begin(v), end(v), 0);
   return make_pair(after - before, i);
}

auto check_general_no_exit(vector<int> v)
{
   clog.rdbuf(nullptr);
   transform(begin(v), end(v), begin(v), [](int n) { return abs(n); });
   auto before = high_resolution_clock::now();
   vector<int>::size_type i = 0;
   for (; i != v.size(); ++i)
   {
      int result;
      if (!process_or_signal(v[i], result)) break;
      v[i] = result;
   }
   auto after = high_resolution_clock::now();
   clog << accumulate(begin(v), end(v), 0);
   return make_pair(after - before, i);
}}

int main()
{
   enum { N = 5000000 };
   auto v = generate_test_data(N);
   cout << "\nChecking for errors every iteration\n\n";
   auto every_it = check_every_iteration(v);
   cout << "Processed " << v.size() << " elements, of which "
        << every_it.second << " led to an error being signalled\n"
        << "Elapsed time: " << duration_cast<microseconds>(every_it.first).count()
        << " microseconds" << endl;
   cout << "\nChecking for errors globally, early exit\n\n";
   auto early_exit = check_general_early_exit(v);
   cout << "Processed " << v.size() << " elements, of which element "
        << early_exit.second << " led to an error being signalled\n"
        << "Elapsed time: " << duration_cast<microseconds>(early_exit.first).count()
        << " microseconds" << endl;
   cout << "\nChecking for errors globally, late exit\n\n";
   auto late_exit = check_general_late_exit(v);
   cout << "Processed " << v.size() << " elements, of which element "
        << late_exit.second << " led to an error being signalled\n"
        << "Elapsed time: " << duration_cast<microseconds>(late_exit.first).count()
        << " microseconds" << endl;
   cout << "\nChecking for errors globally, no exit\n\n";
   auto no_check = check_general_no_exit(v);
   cout << "Processed " << v.size() << " elements\n"
        << "Elapsed time: " << duration_cast<microseconds>(no_check.first).count()
        << " microseconds" << endl;
}
