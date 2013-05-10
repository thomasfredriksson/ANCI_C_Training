/*

Problem

Your car is out of gas, and you want to get home as quickly as possible! 
Fortunately, your home is at the bottom of a hill 
and you (in your car) are at the top of it. 
Unfortunately, there is a car in front of you, and you can't move past it. 
Fortunately, your brakes are working and they are very powerful.

You start at the top of the hill with speed 0 m/s at time 0 seconds. 
Gravity is pulling your car down the hill with a constant acceleration. 
At any time, you can use your brakes to reduce your speed, 
or temporarily reduce your acceleration, by any amount.

How quickly can you reach your home if you use your brakes in the best possible way?

Input

The first line of the input gives the number of test cases, T. 
T test cases follow. 
The first line of each test case contains three space-separated numbers: 
a real-valued number D, the distance in meters to your home down the hill; 
and two integers, N and A. 
The distance D will be given in exactly 6 decimal places.

N lines follow, 
each of which contains two space-separated, 
real-valued numbers: a time ti in seconds, 
and a position xi in meters. 
The ti and xi values will be given in exactly 6 decimal places.

One line follows, with A space-separated, real-valued numbers ai, 
which are accelerations in m/s2. 
The accelerations will be given in exactly 2 decimal places.

The other car's position is specified by the (ti, xi) pairs. 
The car's position at time ti seconds is xi meters measured from the top of the hill 
(i.e. your initial position). 
The car travels at constant speed between time ti and ti+1. 
The positions and times will both be given in increasing order, with t0=0.

For example, if t5=10, x5=20, t6=20, x6=40, 
then 10 seconds after the start, 
the other car is 20 meters down the hill; 
15 seconds after the start, 
the other car is 30 meters down the hill; 
and 20 seconds after the start, 
the other car is 40 meters down the hill.

Output

For each test case, output one line containing "Case #c:", 
where c is the case number (starting from 1). 
Then output A lines, the ith of which contains the minimum number of seconds 
it takes you to reach your home if your acceleration down the hill due to gravity is ai, 
and you use your brakes in the best possible way. 
Answers within an absolute or relative error of 10-6 
of the correct answer will be accepted. 
There should be no blank lines in the output.

Notes

Position and Acceleration: 
An object with a constant acceleration a m/s2 and starting speed of v0 
m/s will move a distance of v0*t + 0.5*a*t^2 after t seconds.

Distance on the slope: 
All the distances and accelerations are given with respect 
to the straight line down the hill. 
They are not, for example, horizontal distances; 
so if your car is accelerating at 2 m/s2 with an initial speed of 0 m/s, 
and the other car is stopped at x=1, 
it will take exactly 1 second to reach the other car.
The other car: 
You may never pass the other car, 
which means that at no time shall your distance down the hill 
be greater than that of the other car. 
It may be equal. 
The cars should be considered as point masses.

Output values: You can print as many decimal places as you like in the output. 
We will read and compare your answers with ours, 
and at that time we will be using 10-6 as a threshold for inaccuracy. 
So 25, 25.0 and 25.000000 are the same from our perspective. 
Trailing zeros after the decimal point does not matter.

Limits

1 ≤ T ≤ 20.
1.0 ≤ D ≤ 104.
1.0 ≤ ai ≤ 9.81.
0.0 ≤ ti ≤ 105.
0.0 ≤ xi ≤ 105.
ti < ti+1.
xi < xi+1.
t0 = 0
xN-1 ≥ D.
Small dataset

1 ≤ N ≤ 2.
1 ≤ A ≤ 10.
Large dataset

1 ≤ N ≤ 2000.
1 ≤ A ≤ 250.
Sample


Input 
 	
Output 
 
3
1000.000000 2 3
0.000000 20.500000
25.000000 1000.000000
1.00 5.00 9.81
50.000000 2 2
0.000000 0.000000
100000.000000 100.000000
1.00 1.01
10000.000000 3 1
0.000000 0.000000
10000.000000 0.100000
10000.100000 100000.000000
1.00
Case #1:
44.7213595
25.000000
25
Case #2:
50000
50000
Case #3:
10140.974143



/*
 * This was a hard problem to wrap your head around, as evidenced by the results. 
 * The final solution was, however, surprisingly simple 
 * (although the argumentation for it is not).

 * Let us first consider a single case, 
 * with a single acceleration value a, and an arbitrarily large N. 
 * Suppose some strategy S1 takes you home in time T. 
 * Obviously a*T^2 / 2 ≥ D, 
 * as a*T^2 / 2 is the distance we travel if we start accelerating immediately and never brake.
 *
   We can propose an alternate strategy S2: 
   first stop and wait for time T - sqrt(2 D/ A), and then start accelerating full speed and never brake.
   This will also bring you home in time T.

   We have to prove that if S1 did not collide with the other car, 
   neither will S2. 
   We prove this by checking that S2 arrives at any point 
   between the top of the hill and your home no earlier than S1.


* Assume S1 was at some point X later than S2. 
   Notice that the speed of S1 coming into X had to be larger than the speed of S2 
   — otherwise even accelerating full speed will not let S1 catch up with S2. 
   But it is impossible to achieve a faster speed at X than S2: 
   strategy S2 accelerated all the way from the top of the hill to X.

   So, now we only need to consider strategies such as S1.
   We now just need to determine how long do we need to wait on the top of the hill.

   One last thing to notice is that 
   if the other car moves at constant speed between xi and xi+1, 
   and our strategy does not put us in front of the other car at xi or at xi+1, 
   then it does not intercept the other car in any intermediate point either. 
   We know that because if we did intercept it in some intermediate point, 
   it would mean that we were moving faster than the other car at the time of interception; 
   and since we're accelerating, and the other car's speed is constant, 
   we would end up in front of it at xi+1. 
   Therefore passing the other car between xi and xi+1 leads to a contradiction, 
   and it must be the case that we don't pass it.

With this reasoning complete, we now have two possible algorithms. 
One is a binary search: 
To check if a given waiting time T is long enough, 
we just need to check all the N points at which the other car can change its speed. 
To achieve a precision of 10-6, 
we will need around 40 iterations in the binary search. 
This means we solve each test case in O(N A) time, 
with the constant 40 hidden in the big-O notation — fast enough.

If we are worried about the constant factor of 40, 
we can also iterate over all points xi, 
and for each calculate the minimum waiting time needed 
not to intercept the other car at this point: ti - sqrt(2xi/a) 
for all i such that xi < D 
(you also have to include the moment where the other car reaches your house). 
This will make our solution run in O(N A) time without the pesky constant.










LD time_needed(LD speed, LD acc, LD distance) {
    if (distance < 1e-9) {
        return 0;
    }
    // x * speed + 0.5 * acc * x^2 == distance
    LD a = 0.5 * acc;
    LD b = speed;
    LD c = - distance;
    LD del = b * b - 4 * a * c;
    return (-b + sqrtl(del)) / (2 * a);
}

void alg() {
    LD distance;
    scanf("%Lf", &distance);

    int n, n_queries;

    scanf("%d %d", &n, &n_queries);

    vector<LD> time(n);
    vector<LD> position(n);
    vector<LD> speed(n);

    for (int i = 0; i < n; ++i) {
        scanf("%Lf %Lf", &time[i], &position[i]);
    }

    while (position[n - 1] > distance) {
        if (position[n - 2] < distance - 1e-9) {
            time[n - 1] = time[n - 2] + ((distance - position[n - 2]) / (position[n - 1] - position[n - 2])) * (time[n - 1] - time[n - 2]);
            position[n - 1] = distance;
            break;
        } else {
            --n;
        }
    }

    static int case_no = 0;
    printf("Case #%d:\n", ++case_no);

    for (int i = 0; i < n_queries; ++i) {
        LD acc;
        scanf("%Lf", &acc);
        LD result = 0;
        for (int j = 0; j < n; ++j) {
            result = max(result, time[j] - time_needed(0, acc, position[j]));
        }
        printf("%.10Lf\n", result + time_needed(0, acc, distance));
    }
}

int main() {
    int d;
    scanf("%d", &d);
    while (d--) {
        alg();
    }
}








template<typename T> T Abs(T x) { return(x < 0 ? -x : x); }
template<typename T> T Sqr(T x) { return(x*x); }

const int INF = (int)1e9;
const LD EPS = 1e-9;
const LD PI = acos(-1.0);

#define DEBUG 0
#define LIMN 2000
#define LIMA 250

int main()
{
	//files
	freopen("in.in","r",stdin);
		if (!DEBUG)
			freopen("out.txt","w",stdout);
	//vars
	int tt,TT;
	LD D;
	int N,A;
	int i,j;
	LD r1,r2,m,s;
	static LD cx[LIMN],cy[LIMN];
	static LD acc[LIMA];
	//testcase loop
	scanf("%d",&TT);
		For(tt,TT)
		{
			//init
			printf("Case #%d:\n",tt+1);
			//input
			scanf("%Lf%d%d",&D,&N,&A);
				For(i,N)
					scanf("%Lf %Lf",&cx[i],&cy[i]);
				For(i,A)
					scanf("%Lf",&acc[i]);
			//ignore car past destination
				For(i,N)
					if (cy[i]>D)
					{
						s=(cx[i]-cx[i-1])/(cy[i]-cy[i-1]);
						cy[i]=D;
						cx[i]=cx[i-1] + s*(cy[i]-cy[i-1]);
						N=i+1;
						break;
					}
			//answer each query
				For(i,A)
				{
					//binary search on when to leave
					r1=0,r2=100000;
						while (r2>r1+EPS)
						{
							//get middle
							m=(r1+r2)/2;
							//try leaving here
								For(j,N)
									if ((cx[j]>m) && (0.5*acc[i]*Sqr(cx[j]-m)>cy[j]+EPS))
										goto Bad;
							//good - go earlier
							r2=m;
							continue;
							//bad - go later
Bad:;
							r1=m;
						}
					//output
					printf("%.7Lf\n",r1 + sqrt(2*D/acc[i]));
				}
		}
	return(0);
}

*/

void Pr16Solve()
{
	int tt; // test case number
	int i, r1, r2, m, j;
	int D;  // D, the distance in meters to your home down the hill; 
	int N;  // N lines follow, 
            // each of which contains two space-separated, 
            // real-valued numbers: a time ti in seconds, 
            // and a position xi in meters. 
	int A;
    int tt[1000];
	int xx[1000];
	int acc[1000];
	int s, speed;

    scanf("%d",&TT);
    for (tt = 0; tt < TT; tt++)
	{
	    scanf("%%d%d",&D,&N,&A);
		for (i = 0; i < N; i++)
		  scanf("%d %d",&tt[i],&xx[i]);
		for (i = 0; i < A; i++)
		  scanf("%d",&acc[i]);
		//ignore car past destination
		for (i = 0; i < N; i++)
	      if (xx[i] > D)
		  {
		     /* Speed for the last  */
		     speed= (xx[i]-xx[i-1])/(tt[i]-tt[i-1]);
			 xx[i]=D; // We have found the last car position. We do not need to
                      // calculate it when xx[i] > D. We set the last xx[i] to D
                      // [xx[i-1]...D...xx[i]]
                      // 
             /* D-xx[i-1]=speed*(tt[i]-tt[i-1]) */
			 tt[i]=tt[i-1]+(D-xx[i-1])/speed;
			 N=i+1;
			 break;
		  }

	    //answer each query
		for (i = 0; i < N; i++)
		{
		   //binary search on when to leave
		   r1=0,r2=100000;
		   while (r2>r1+1)
		   {
			   //get middle
			   m=(r1+r2)/2;
               //try leaving here
			   For(j,N)
				 if ((cx[j]>m) && (0.5*acc[i]*Sqr(cx[j]-m)>cy[j]+EPS))
		   }
		}
	}
}