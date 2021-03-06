/*
 
 You are walking through the mountains. It turns out that in this mountain range 
 there is a peak every kilometer, and there are no intermediate peaks. 
 On every peak, you lie down for a rest, look forward, 
 and perceive one of the peaks in front of you to be the highest one. 
 
 The peak that looks like it's the highest might not really be the highest, for two reasons: 
 there could be a higher peak that is obscured by another peak that's closer to you, 
 and not as high; or you could be looking down, 
 and a faraway peak could look higher than a nearby one.

  To be precise, when we say that peak B looks like it's 
  the highest from peak A we mean that B is further down the road than A; 
  all peaks between A and B are below the line connecting the peaks A and B; 
  and all the peaks that are further than B are below or on this line.

  You don't know how high each peak is, 
  but you have a very good memory; 
  
  you've been on all the peaks; 
  and you remember which peak looks like it's the highest from each of them. 
  You would like to invent a set of heights for the peaks 
  that is consistent with that information. 
  Note that you were lying down when looking, 
  so we assume you always looked from the ground level on each peak. 

  In this example, the fourth peak looks like it's the highest 
  from the first and third peaks. 
  When you're lying on the second peak, you can't see the fourth peak; 
  the third one obscures it, and looks like it's the highest. 

  Input

  The first line of the input gives the number of test cases, T. 
  T test cases follow. Each test case consists of two lines. 
  The first contains one number, N, the number of peaks in the range. 
  You began your trip on peak 1 and went forward to peak N. 
  The next line contains N-1 numbers xi. 
  The i-th number denotes the index of the peak 
  that appeared to be the highest from peak i 
  (note that peak N is the last peak, so there are no other peaks to see from there). 

  Output

  For each test case, output one line containing 
  "Case #n: y1 y2 ... yN", 
  where n is the case number (starting from 1) 
  and yi is the height of the i-th peak. 
  You can output any solution agreeing with the input data, 
  except that all the heights you output have to be 
  integers between 0 and 109, inclusive. 

  If no solution is possible, output "Case #n: Impossible" instead. 
  Limits

   1 ≤ T ≤ 30.
   i < xi ≤ N. 
   Small dataset

   2 ≤ N ≤ 10. 
   Large dataset

   2 ≤ N ≤ 2000. 

  Sample
  Input 
  	
  Output 
  
   4
   6
   2 3 4 5 6
   4
   4 4 4
   4
   3 4 4
   4
   4 3 4

 */

const int TOP = 1000000000;

/* Let's describe How the algorythm works  
 * for example: H = [1, 1], N = 2 + 1 = 3 
 * first = 1, first - 1 = 0, H1Count = 0;
 * out1 = solve(H1) = [max]
 * H2Count = 3 - 1 - 1 = 1
 *  for (int n=first; n<N-1; ++n) H2[n-first] = H[n]-first;
 * n = 1, n < 2;  n = 1..1  H2 = [0]
 * out2 = solve(H2) = [max, max]
 *  
 * out = [max,*,*] 
 * 
 * for (int n=1; n<first; ++n) out[n] = out1[n-1] - first + n;
 * for (int n=first; n<N; ++n) out[n] = out2[n-first];
 * n = 1; n < 3; n++ :  out = [max, max, max]
 * Bad example. Let's take another example 
 * [2,2,3] = H.
 * first = H[0] = 2, N - 1 = 3, n = 2, n < 3
 * H1 = [1], H2 = [2]
 * out1 = [max], out2 = [max] 
 * out = [max,*,*,*] n = 1; n < 2 = 1: out[1] = max - (first - n) = max - 1
 * n = 2; n < N = 
 * 
 */
int* Pr13Solve(int* H, int HCount)
{
   int N = HCount + 1;
   int out[100];
   int zero[1];
   int outCount = N;
   int i, n;
   int first;
   int H1[100];
   int H2[100];
   int out1[100];
   int out2[100];
   int H1Count;
   int H2Count;
   for (i = 0; i < outCount; i++) out[i] = TOP;
   if (N <= 2) return out;
   first = H[0];
   H1Count = first - 1;
   H2Count = N - 1 - first;
   for (n = 1; n < first; ++n) 
   {
        if (H[n] > first) return zero;
        H1[n-1] = H[n]-1;
   }

   for (n = first; n < N - 1; ++n) 
	   H2[n-first] = H[n]-first;

   //out1 = Pr13Solve(H1, H1Count);

  /* if (out1.empty()) return vector<int>(0);
    vector<int> out2 = solve(H2);
    if (out2.empty()) return vector<int>(0);
    for (int n=1; n<first; ++n) out[n] = out1[n-1] - first + n;
    for (int n=first; n<N; ++n) out[n] = out2[n-first];
    return out;*/
}

