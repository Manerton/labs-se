using System;
using System.Threading;

namespace Lab_1
{
    class Program
    {
        static void func(object label)
        {
            double x = 12345.6789;

            for (int i = 0; i < 10000; i++)
            {
                for (int j = 0; j < 10000; j++)
                {
                    x = Math.Sqrt(x);
                    x = x + 0.000000001;
                    x = Math.Pow(x, 2);
                }
            }

            Console.WriteLine((string)label + ": " + x.ToString());
        }

        static void Main(string[] args)
        {
            DateTime startTime = DateTime.Now;

            // Создание потоков
            Thread f1 = new Thread(new ParameterizedThreadStart(func));
            f1.Start("A");

            Thread f2 = new Thread(new ParameterizedThreadStart(func));
            f2.Start("B");

            Thread f3 = new Thread(new ParameterizedThreadStart(func));
            f3.Start("C");

            Thread f4 = new Thread(new ParameterizedThreadStart(func));
            f4.Start("D");

            Thread f5 = new Thread(new ParameterizedThreadStart(func));
            f5.Start("E");

            Thread f6 = new Thread(new ParameterizedThreadStart(func));
            f6.Start("F");

            Thread f7 = new Thread(new ParameterizedThreadStart(func));
            f7.Start("G");

            Thread f8 = new Thread(new ParameterizedThreadStart(func));
            f8.Start("H");

            Thread f9 = new Thread(new ParameterizedThreadStart(func));
            f9.Start("I");

            // Ожидание завершения всех потоков
            f1.Join();
            f2.Join();
            f3.Join();
            f4.Join();
            f5.Join();
            f6.Join();
            f7.Join();
            f8.Join();
            f9.Join();

            DateTime endTime = DateTime.Now;

            Console.WriteLine((endTime - startTime).TotalSeconds);
            Console.ReadLine();
        }
    }
}
