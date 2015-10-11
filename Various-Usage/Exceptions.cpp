class invalid_data {};

int process_or_throw(int n)
{
   if (n < 0) throw invalid_data{};
   return n * 2;
}

void check_general_no_exit(int v[], int n)
{
   int i = 0;
   try
   {
      for (; i != n; ++i)
         v[i] = process_or_throw(v[i]);
   }
   catch (...)
   {
   }
}
