/*
 Formula:
 P(woken up) = 
  (1 - p0)(1 - (1 - p1)(1 - p2)...(1 - pk))
+ p0(1 - p1)(1 - (1 - p2)(1 - p3)...(1 - pk))
+ p0p1(1 - p2)(1 - (1 - p3)...(1 - pk))
+ ....
+ p0p1p2................pk-2(1 - pk-1)(1 - (1 - pk))

(1) - after p0 he is asleep and he is awake afetr 1 or 2 or 3 ...
(2) - he is awake after 0 then is asleep after 1 and is awake after 2 or 3 or 4...
(3) - he is awake after 0 and after 1, then he is asleep after 2 and 
      is awake after 3 or 4 or 5 or .... 
.....
(k) - he is awake after 0 and after 1 and after 2 and ... and after k - 2 
      then he is asleep after k - 1 and is awake after k

 Another way is to compute this firmula using Markov processes
 Illia can be in 3 possible states: [awake, asleep, woken up]
 The initial Illia's state is [1, 0, 0]
 The possible state vectors are: 
 [1, 0, 0] - awake    In column definition  [1] [0] [0]
 [0, 1, 0] - asleep                         [0] [1] [0]
 [0, 0, 1] - woken up                       [0] [0] [1]
 
 Lets take ith activity. 
 pi - is a probability that Illia will be awake
 (1 - pi) - is a probability that Illia will be asleep.
 0 - is a probability that woken up. It can be explained as to be woken up
     people need to be asleep
 If the current state is awake. That means that Illia can be asleep
 and can stay awake. It can be explained that the noise is so strong that Illia canno get sleep.
 Or performing some activity Konstantin has noticed that ni times Illia was awake and ki he was asleep.
 So he calculated the probability: ni/ki. 
 If the state is asleep
 0 - is a probability that Illia will be awake. This is 0 because it is impossible 
     to move from asleep state to awake state without being woken up.
 (1 - pi) - is a probability that Illia will be asleep. He is asleep and than he stays to be asleep
 pi - if he is awake after ith activity, that means that he has been woken up.
If we is woken up he can only stay to be woken up further.
 pi      0      0 
 1 - pi  1 - pi 0    is a stohastic matrix matrix of transitions. 
 0       pi     1
 Lets compute 2 adjacent stohastic matrix - it can describe a behaviour of Konstantin
 performing 2 sequentual activities i and i + 1
 |pi   0    0| |pi+1   0      0| |c00 0   0| |p   0   0| |c00p
 |1-pi 1-pi 0|*|1-pi+1 1-pi+1 0|=|c10 c11 0|*|1-p 1-p 0|=|c10
 |0    pi   1| |0      pi+1   1| |c20 c21 1| |0   p   1| |
 C[0,0] = pipi+1 + 0 + = pipi+1
 C[0,1] = 0
 C[0,2] = 0
 C[1,0] = (1-pi)pi+1 + (1-pi)(1-pi+1)
 C[1,1] = (1-pi)(1-pi+1)
 C[1,2] = 0
 C[2,0] = pi(1-pi+1)
 C[2,1] = pi(1-pi+1) + pi+1
 C[2,3] = 1
 |1 0 0| |p   0   0|   |p   0   0|
 |0 1 0|*|1-p 1-p 0| = |1-p 1-p 0|
 |0 0 1| |0   p   1|   |0   p   1|
 Continue this process for getting a recurrent sequence
 |x  0  0| |p   0   0| |xp        0        0|
 |y  s  0|*|1-p 1-p 0|=|yp+s(1-p) s(1-p)   0|
 |z  t  1| |0   p   1| |zp+t(1-p) t(1-p)+p 1|
We are interested in a probability when Illia was awake and after n
processes he is woken up 

t = t(1-p)+p, 
z = zp+t(1-p)

1 - t = 1 - t(1 - p) - p = (1 - t)(1 - p) 
t = 1 - (1 - t)(1 - p)

1. z = 0 t = 0
2. z = 0 t = 1 - (1 - p0)
3. z = (1 - p1)(1 - (1 - p0))
   t = 1 - (1 - p0)(1 - p1)
4. p2(1 - p1)(1 - (1 - p0)) + (1 - p2)(1 - (1 - p0)(1 - p1))
   t = 1 - (1 - p0)(1 - p1)(1 - p2)
5.  p3(p2(1 - p1)(1 - (1 - p0)) + (1 - p2)(1 - (1 - p0)(1 - p1)))
  + (1-p3)(1 - (1 - p0)(1 - p1)(1 - p2)) =

  z =  p3p2(1 - p1)(1 - (1 - p0)) +
       p3(1 - p2)(1 - (1 - p0)(1 - p1)) +
       (1-p3)(1 - (1 - p0)(1 - p1)(1 - p2))

  t = 1 - (1 - p0)(1 - p1)(1 - p2)(1 - p3)

4. z = p0(1-p1)p2 + (p0(1-p1)+p1)(1-p2) t = (1-p2)(p0(1-p1)+p1) + p2
Lets see is swapping pi+1 and pi can change the value.
ti = 
|x  0  0| |x1  0    0| |xx1
|y  s  0|*|y1  s1   0|=|yx1+sy1
|z  t  1| |z1  t1   1| |zx1+ty1+z1
So we need keep track of zx1+ty1+z1

ti = t(1-pi)+pi, zi = zpi+t(1-pi)
ti+1 = ti(1-pi+1)+pi+1 = (t(1-pi)+pi)(1-pi+1)+pi+1
zi+1 = zipi+1+ti(1-pi+1) = (zpi+t(1-pi))pi+1 + (t(1-pi)+pi)(1-pi+1)=
= zpipi+1 + t(1-pi)pi+1 + t(1-pi)(1-pi+1) + pi(1-pi+1) = 
= zpipi+1 + tpi+1 - tpipi+1 + t(1 - pi - pi+1 + pipi+1) + pi - pipi+1=
= pipi+1(z - t + t - 1) + tpi+1 - tpi+1 - tpi + pi + t = 
= pipi+1(z - 1) + (1 - t)pi + t
Here we can make an conclusion that if we swap i and j we will receive differences
Curr[i,j] = pipi+1(z - 1) + (1 - t)pi + t
Curr[j,i] = pi+1pi(z - 1) + (1 - t)pj + t
Curr[j,i] - Curr[i,j] = (1 - t)(pj - pi) < 0 pj < pi
t = 0;          1 - t = 0
t = p0          1 - t = 1 - po
t = p0(1-p1)+p1 1-  t = 1 - p0(1-p1) - p1 = (1-p0)(1-p1)
1 - p2(1-p0)(1-p1) - p1 = (1-p0)...(1-p2)
*/ 

double** CreateSholasticMatrix(double Probability)
{
   double P = Probability;
   double Result[3][3] = 
   { { P    , 0    , 0 },
     { 1 - P, 1 - P, 0 },
     { 0    , P    , 0 }};
   return Result;
}

void SquareMatrixMultix(
	double Left[3][3], 
	double Right[3][3], 
	double Result[3][3])
{
	double c;
    int i, j, t;

    for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			c = 0;
			for (t = 0; t < 3; t++)
			{
               c += Left[i][t] * Right[t][j];    
			}
			Result[i][j] = c;
		}
	}
}

/* Probs - is a sorted array of probabilities in decrease order  
 * For each probability we have a sholastic matrix Si. Prefixes matrix
 * multiplications looks like:
 * Prefix0 = E1
 * Prefix1 = S1
 */
void ComputePrefixex(double Probs[], int n, double** Prefixes[])
{
  
}