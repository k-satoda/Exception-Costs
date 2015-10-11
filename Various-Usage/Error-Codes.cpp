bool process_or_signal(int n, int &result)
{
   if (n < 0) return false;
   result = n * 2;
   return true;
}

bool check_general_no_exit(int v[], int n)
{
   int i = 0;
   for (; i != n; ++i)
   {
      int result;
      if (!process_or_signal(v[i], result)) return false;
      v[i] = result;
   }
   return true;
}
