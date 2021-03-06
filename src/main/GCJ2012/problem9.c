
/*

Problem

You just moved from your hometown to a big metropolitan city! 
You love everything about your new environment, except for the food. 
Your hometown provides the best food in the region (called "quality food") 
and you sure will miss it.


Fortunately, the largest restaurant in your hometown 
provides a food delivery service. 
You can purchase any amount of food in one delivery. 

There is a constant delivery fee for every delivery, 
regardless of the amount of food purchased in the delivery.

This restaurant serves different types of food. 
Each type of food has two properties: a price-per-meal, and a time-to-stale. 

One "meal" of food will feed you for one day; 
once a meal has been eaten, it cannot be eaten again. 

The time-to-stale of a type of food is the maximum number of days 
for which that food can still be eaten, counting from when you received it. 
A time-to-stale of zero means you must eat that type of food on the day of delivery.

In a single delivery you can purchase as many different types of food, 
and as many meals of each of those types, as you have money for. 

Note that if a particular type of food has a time-to-stale of t, 
it doesn't make any sense to order more than t+1 meals of that food in one delivery: 
at least one meal would go stale before you could eat it.

This restaurant has a very fast delivery service, 
so you will receive all the food in a delivery on the same day that you purchased it, 
and you may eat some of the food on the same day. 
Food delivery is the only way for you to receive quality food.

Given an amount of money, which you can spend on meal prices and delivery fees, 
what is the maximum number of days for which you can eat quality food every day?

Input

The first line of input gives the number of test cases, T.
T test cases follow. 
Each test case will begin with three integers, M, F and N, 
denoting the amount of money you have, the delivery fee, 
and the number of types of food provided by the restaurant, respectively. 
N lines follow, each will consist of two integers, Pi and Si, 
denoting respectively the price-per-meal and time-to-stale of one type of food.


Output


For each test case, output one line containing "Case #x: y", 
where x is the case number (starting from 1) 
and y is the maximum number of days that you can keep eating at least one meal 
of quality food everyday.


Limits

1 ≤ T ≤ 50.
1 ≤ F ≤ M.
1 ≤ N ≤ 200.
1 ≤ Pi ≤ M.
Small dataset

0 ≤ Si ≤ 2,000,000.
1 ≤ M ≤ 2,000,000.
Large dataset

0 ≤ Si ≤ 1018.
1 ≤ M ≤ 1018.
Sample


Input 
 	
Output 
 
3
32 5 2
5 0
10 2
10 10 1
10 10
10 1 1
1 5
Case #1: 3
Case #2: 0
Case #3: 8


An example scenario for the first case is by purchasing 
one meal of the first type and one meal of the second type 
during your first day in the city (costing a total of 20). 

Eat the first type of food that day, 
and eat the second type the next day. 
During your third day, 
purchase one meal of the first type and eat it on the same day. 
This accounts for three days.







Multiple Solutions


Sometimes our problemsetters like to come up with big, 
complicated solutions to their problems, 
and sometimes it turns out that there are simpler alternatives. 
We'll present two solutions here, 
including the simple one that many problems solvers used, 
and the more complex one used by the author.


How much does a single delivery cost?

Let's look at the cost of a delivery containing K meals. 

It's easy to see that we should order 

the cheapest meal with a time-to-stale of at least 0, 
the cheapest meal with a time-to-stale of at least 1, 
the cheapest meal with a time-to-stale of at least 2, 

and so on until we order the cheapest meal with a time-to-stale of at least K. 
Since K could be very large, we'll solve this problem in O(Nlog(N)).

First we'll sort the kinds of food by price, 
and construct an array that contains 
[(0, 0), (d1, c1), (d2, c2), ...], 

where we should buy food that costs ci for the meals between 
di-1 and di after the delivery arrives. 

We can then process that into another array containing (di, Ci), 
where Ci is the total cost of a single delivery that lasts di days, 
and (Ci-Ci-1) / (di-di-1) = ci.

We only had to do those above steps once. 
Now that we have that array, it's an O(log(N)) 

binary search (or a simpler linear search, 
since we'll find that we don't need 
the efficiency for either of our solutions) 

to find out how much it costs to have a delivery that lasts K days: 
find i such that di-1 ≤ K < di, and charge Ci-1 + (K-di-1)*ci-1.

We'll call that function SingleDeliveryCost(days), 
and for convenience later in this editorial 
we'll also define SingleDayCost(day) 
to be the cost to buy the cheapest meal with a time-to-stale of at least day. 

Observe that SingleDayCost(a) ≤ SingleDayCost(b) if a ≤ b, 
and that SingleDeliveryCost(days+1) = SingleDeliveryCost(days) + SingleDayCost(days+1).

Deliveries should be (almost) the same size

Let's show that all of your deliveries should be almost the same size. 
Let's suppose that we have a solution that has a delivery, A, 
containing a meals, and another delivery, B, 
that contains b meals, with b ≥ a+2. 

Then the cost for those two deliveries is SingleDeliveryCost(a) + SingleDeliveryCost(b). 
If instead we increased delivery A's size to a+1 
and decreased delivery B's size to b-1, 
we'd have the same total number of days, but the delivery cost would be:

SingleDeliveryCost(a+1) + SingleDeliveryCost(b-1)
= SingleDeliveryCost(a) + SingleDayCost(a+1) + SingleDeliveryCost(b) - SingleDayCost(b-1)
≤ SingleDeliveryCost(a) + SingleDeliveryCost(b).


This shows that all our deliveries should be of size a or a+1 for some value of a.
Deliveries should be of an "interesting" size

Let's say that we want to buy a total of D days' worth of food, 
and we want to know how big our deliveries should be. 
We'll start by considering a number of deliveries X that 
we're going to show is "uninteresting": 
one such that if we look the size of the largest delivery, 
ceil(D/X), 
then ceil(D/(X-1)) = ceil(D/X) = ceil(D/(X+1)). 
In such a case, adding a delivery changes the cost by 

SingleDeliveryCost(ceil(D/X)) - ceil(D/X)*SingleDayCost(ceil(D/X)). 

If we add a delivery, we'll change the cost by the negation of that amount. 
One of those two quantities has to be non-negative, 
which means that we never need to consider making X deliveries.

That means that we only need to look at numbers of deliveries 
such that if we changed the number of deliveries, 
we'd be changing SingleDayCost(ceil(D/X)). 

There are only 2N such delivery sizes, 
and trying all of them solves the problem in O(N log(N)).



The other approach


A competitor who couldn't convince himself or herself of the above 
still has a good chance of solving the problem. 

We'll start by presenting a trick ("Changing the Question") 
that you can use whether or not you have the above realization–
it might make the math easier–and a totally different approach that, admittedly, requires some math of its own.
Changing the Question

An observation we can make is that instead of solving the problem that was asked, 
we can play a trick to let ourselves solve a simpler problem: 
"Can I eat quality food for D days?" 
We do this by binary searching on the answer. 

We know the answer is between 0 and M, 
so we can start by solving that problem O(log(M)) times.


How many deliveries should there be?

If the logic in the previous solution was too complicated, 
here's an alternative that probably isn't going to make you any happier. 
On the other hand, if you were thinking that there weren't enough logarithmic factors 
in the complexity analysis, or enough -ary searches, this could make you happy. 
This was our original solution for the problem, 
and it makes the I/O author perversely proud to have been part of making 
a problem to which a binary search in a ternary search in a binary search 
is a perfectly reasonable solution.

The argument here is that we can ternary search for the number of deliveries 
that minimizes the cost to buy quality food for D days. 
In order for a ternary search to be possible, 
the function has to be strictly decreasing, 
then strictly increasing (or vice versa). 

In this case, the domain of the function 
is from where the number of deliveries X 
is the minimum possible number at which we could order D meals 
(regardless of how expensive they are), to X=D. 

We'll show that the function decreases, then increases over this domain.

First let's take the SingleDayCost function and extend it over the real numbers. 
We already know what it is for the integers; 
let's just linearly interpolate for the real numbers in between, 
and call the result G. 
This has the nice result that the cost for X deliveries and D days, 
which we'll call H(X), 
is H(X) = G(D/X)*X + X*F.

Now, we're about to start taking derivatives, 
and although G is continuous, it isn't differentiable. 

There are a couple of ways of getting around this, 
and we're going to take a physicists' route. 

We'll define a function G'' to be a sum of delta functions 
such that the double-integral of G'' over X is our original G. 

Then we'll define G' to be G'''s integral, 
and G to be G''s integral. 
That lets us say that the first derivative of G 
is non-negative everywhere, and so is its second derivative. 
Don't try this at home, folks, especially if you live in a math class.

What we're really interested in doing here is proving 
that H(X) is decreasing and then increasing,
which we can do by proving that it has a positive second derivative:

H(X) = G(D/X)*X + X*F
H'(X) = G(D/X) - X*G'(D/X)*X-2 + F
H'(X) = G(D/X) - G'(D/X)/X + F
H''(X) = -X-2G'(D/X) + X-2G'(D/X) + G''(D/X)X-3
H''(X) = G''(D/X)X-3 ≥ 0
Therefore H(X) is decreasing and then increasing 
(or just decreasing or increasing, which is also fine), 
and can be ternary searched on for the minimum cost of surviving D days. 
This algorithm's complexity works out to O(Nlog(N) + log2(M)log(N)).


*/

/*
3
32 5 2
5 0
10 2
10 10 1
10 10
10 1 1
1 5
Case #1: 3
Case #2: 0
Case #3: 8
*/


struct Offer 
{
    int TimeToStale;
    int Price;
};

struct DeliveryItem
{
	int Day;
	int Cost;
    int Price;
};

void Pr9Swap(struct Offer Offers[], int i, int j)
{
    struct Offer temp;
	temp = Offers[i];
    Offers[i] = Offers[j];
    Offers[j] = temp;
}

/* Here we need to choose what food is better for us.
 * What food do we need to eat first? 
 *  Of course the food with the smaller time-to-stale
 *  we need to it first.
 * When food's time-to-stale are the same we prefer to
 * choose the more expensive one 
 */ 
int CompareOffer(struct Offer* A, struct Offer* B)
{
	return A->Price < B->Price; 
}

void Pr9HeapSort(struct Offer Offers[], int start, int end)
{
	int medium, i, j;

	if (start > end)
	{
		return;
	}

	medium = (start + end) / 2;
	Pr9Swap(Offers, start, medium);

    i = start + 1;
    j = start + 1;

	for (; i <= end; i++)
	{
	   if (CompareOffer(&Offers[i], &Offers[medium]))
	   {
           Pr9Swap(Offers, i, j++);
	   }    
	}
  
    Pr9Swap(Offers, start, j - 1);
    HeapSort(Offers, start, medium - 1);
    HeapSort(Offers, medium + 1, end);
}


/* By this function we can find out how much 
 * the single delivery which lasts one day
 * will cost given a specific sum of money.
 */
int SingleDayCost(
    struct DeliveryItem DeliveryIngredients[],
	int DeliveryCount,
	int Day,
	int Money)
{
    /* min_meals array contains meals order in a single purchase
	 * For example: [di, ci]  di - a day when we srart eating meal ci
	 * meals param is a number of days we want to live using one purchase
	 * money is money we have for this.
	 */
	int TheClosestItemIndex;
    struct DeliveryItem TheClosestItem;
	int CanAfforsTheDay;
    int i;
    /* Loop through all purchase days and find the day 
	 * just after meals.
	 */
	for (i = 0; i < DeliveryCount; i++)
	{
		if (DeliveryIngredients[i].Day >= Day)
		{
			break;
		}
	}

	/* We have found the index i inside delivery ingredients when:
	 * DeliveryIngredients[i].Day <= Days < DeliveryIngredients[i + 1].Day
	 */
    TheClosestItemIndex = i - 1;
    TheClosestItem = DeliveryIngredients[TheClosestItemIndex];
	/* Money - is allocated money for the purchase
	 * TheClosestItem.Cost - how much costs to live until the closest item
	 * DaysAfterClosest - How many days we can live after the closest day 
	 * taking into account that one meal feeds us for one day
	 */
	CanAfforsTheDay = Money >= TheClosestItem.Price;

    if (!CanAfforsTheDay) 
	{
		/* Money + 1 indicates that we do not have enough money 
		 * to afford this sindle delivery 
		 */
        return Money + 1; 
    } 

    return TheClosestItem.Price;
}

/* By this function we can find out how much 
 * the single delivery which lasts a certain number of days
 * will cost given a specific sum of money.
 */
int SingleDeliveryCost(
	struct DeliveryItem DeliveryIngredients[],
	int DeliveryCount,
    int Days, 
	int Money) 
{
	/* min_meals array contains meals order in a single purchase
	 * For example: [di, ci]  di - a day when we srart eating meal ci
	 * meals param is a number of days we want to live using one purchase
	 * money is money we have for this.
	 */
	int TheClosestItemIndex;
    struct DeliveryItem TheClosestItem;
	int DaysAfterClosest;
	int CanAffordTheReminderAfterCloset;
	int ReminderDuration;
	int ReminderCost;
    int i;
    /* Loop through all purchase days and find the day 
	 * just after meals.
	 */
	for (i = 0; i < DeliveryCount; i++)
	{
		if (DeliveryIngredients[i].Day >= Days)
		{
			break;
		}
	}

	/* We have found the index i inside delivery ingredients when:
	 * DeliveryIngredients[i].Day <= Days < DeliveryIngredients[i + 1].Day
	 */
    TheClosestItemIndex = i - 1;
    TheClosestItem = DeliveryIngredients[TheClosestItemIndex];
	/* Money - is allocated money for the purchase
	 * TheClosestItem.Cost - how much costs to live until the closest item
	 * DaysAfterClosest - How many days we can live after the closest day 
	 * taking into account that one meal feeds us for one day
	 */
	DaysAfterClosest = (Money - TheClosestItem.Cost) / TheClosestItem.Price;
    ReminderDuration = Days - TheClosestItem.Day;

	CanAffordTheReminderAfterCloset = DaysAfterClosest >= ReminderDuration;

    if (!CanAffordTheReminderAfterCloset) 
	{
		/* Money + 1 indicates that we do not have enough money 
		 * to afford this sindle delivery 
		 */
        return Money + 1; 

    } 
	
	ReminderCost = ReminderDuration * TheClosestItem.Price;

    return TheClosestItem.Cost + ReminderCost;
}

/* 
 * Observe that SingleDayCost(a) ≤ SingleDayCost(b) if a ≤ b, 
 * and that SingleDeliveryCost(days+1) = SingleDeliveryCost(days) + SingleDayCost(days+1).
 * 
 * We must prove that the deliveries must be almost the same size.
 * Let's suppose that we have a delivery A which lasts a days or has a meals 
 * and a delivery B lasts b days or has b meals and b ≥ a+2. 
 *
 * Then the total cost for those two deliveries is 
 *  SingleDeliveryCost(a) + SingleDeliveryCost(b). 
 *
 * If instead we increased delivery A's size to a+1 
 * and decreased delivery B's size to b-1, 
 * we'd have the same total number of days, but the delivery cost would be:
 *
 *  SingleDeliveryCost(a+1) + SingleDeliveryCost(b-1)
 *   =   SingleDeliveryCost(a) + SingleDayCost(a+1) 
 *     + SingleDeliveryCost(b) - SingleDayCost(b-1)
 *   = SingleDeliveryCost(a) + SingleDeliveryCost(b) + 
 *     + (SingleDayCost(a+1) - SingleDayCost(b+1))
 *  
 *  if a < b then SingleDayCost(a) < SingleDayCost(b)
 *  This can be proved easy: if a < b we choose a cheaper meal
 *  because we have sorted them by price.
 *  So we need to choose deliveries almost the same size.
 */

/*
 * NDays - the number of days to live
 * NDeliveries - the number of deliveries 
 *
 * We want to find the minimal sum of money 
 * needed to live for a certain number of days
 * and always make a certain number of deliveries.
 * 
 */
int GetCost(
    struct DeliveryItem DeliveryIngredients[],
	int DeliveryCount,
    int NDays, 
	int NDeliveries,
	int Money) 
{
	int TotalCost;
	/* Given the rule that we need to divide deliveris on
	 * the same sizes. So we need to calculate the duration
	 * of each delivery.
	 * Example: NDays = 10, NDeliveries = 3
	 * MealsPerDelivery = 10/3 = 3. The reminder is 1.
	 * We need to distribute the reminder among deliveries
	 */
    int MealsPerDelivery = NDays / NDeliveries;
    /* This is a number of days we need to distribute 
	 * among other deliveries.
	 */
    int BigDeliveries = NDays % NDeliveries;
    /* This is a number of deliveries to which 
	 * we do not need to add  */
    int SmallDeliveries = NDeliveries - BigDeliveries;
    /* The cost of a big delivery */
    int BigDeliveryCost = 
		  SingleDeliveryCost(
	           DeliveryIngredients,
	           DeliveryCount,
               MealsPerDelivery + 1, 
	           Money);
	/* The cost of small delivery */
    int SmallDeliveryCost = 
          SingleDeliveryCost(
	           DeliveryIngredients,
	           DeliveryCount,
               MealsPerDelivery, 
	           Money);
    /* The max number of big deliveries we can afford */
	int MaxBig = Money / (1 > BigDeliveryCost ? 1 : BigDeliveryCost) ;
    int MaxSmall = Money / (1 > SmallDeliveryCost ? 1 : SmallDeliveryCost) ;
	/* SmallDeliveryCost is 0 only when MealsPerDelivery is 0
	 * or when NDays / NDeliveries = 0: NDays < NDeliveries
	 */
    if (SmallDeliveryCost == 0) 
	{
        MaxSmall = SmallDeliveries + 1;
    }
    /* BigDeliveryCost is 0 only when NDays % NDeliveries = 0
	 * or when NDays = k * NDeliveries; k = 1, 2, 3, ... 
	 * 
	 */
    if (BigDeliveryCost == 0) 
	{
        MaxBig = BigDeliveries + 1;
    }
    /* If we need to buy BigDeliveries amount of food but we
	 * we can not afford this quantity. In this case we return Money + 1
	 */
    if (BigDeliveries > MaxBig || SmallDeliveries > MaxSmall) 
	{
        return Money + 1;
    } 
	
	TotalCost = BigDeliveries * BigDeliveryCost + SmallDeliveries * SmallDeliveryCost;
    
	if (TotalCost < Money + 1)
	{
		return TotalCost;
	}

    return Money + 1;
}

/* SingleDayCost(a) ≤ SingleDayCost(b) if a ≤ b, 
 * SingleDayCost is an increasing function
 * SingleDeliveryCost(days+1) = SingleDeliveryCost(days) + SingleDayCost(days+1)
 * SingleDeliveryCost is also an increasing function
 *
 * First let's take the SingleDayCost function and extend it over the real numbers. 
 * We already know what it is for the integers; 
 * let's just linearly interpolate for the real numbers in between, 
 * and call the result G. 
 * This has the nice result that the cost for X deliveries and D days, 
 * which we'll call H(X), 
 * is H(X) = G(D/X)*X + X*F. 
 * X - is the number of deliveries, D is the number of days to live
 * D/X is the length of the delivery. D/X is a real value and G - is a function
 * after interpolation. F is a fee for a delivery.
 *
 * That lets us say that the first derivative of G 
 * G is always increasing - that means the first derivative is more than zero
 * But the differences delta(G) is also increasing - that means 
 * the second derivative is more than zero.
 * is non-negative everywhere, and so is its second derivative. 
 * 
 *  H(X) = G(D/X)*X + X*F
 *  H'(X) = G(D/X) - X*G'(D/X)*X-2 + F
 *  H'(X) = G(D/X) - G'(D/X)/X + F
 *  H''(X) = -X-2G'(D/X) + X-2G'(D/X) + G''(D/X)X-3
 *  H''(X) = G''(D/X)X-3 ≥ 0
 *
 *  Therefore H(X) is decreasing and then increasing 
 *  (or just decreasing or increasing, which is also fine), 
 *
 */
int GetMinCost(
	struct DeliveryItem DeliveryIngredients[],
	int DeliveryCount,
	int Days, 
	int Money,
	int DeliveryFee) 
{
	int left;
	int leftThird;
	int right;
	int rightThird;
	int center;
    int fLeft;
	int fRight;

    if (Days == 0) 
	{
        return 0;
    }

    left = 1;
    right = Money + 1;

    while (right - left > 1) 
	{
        leftThird = left + (right - left) / 3;
        rightThird = right - (right - left) / 3;

        center = (left + right) / 2;

        fLeft = GetCost(DeliveryIngredients, DeliveryCount, Days, leftThird, Money) 
			   + DeliveryFee * leftThird;

        fRight = GetCost(DeliveryIngredients, DeliveryCount, Days, rightThird + 1, Money) 
			   + DeliveryFee * (rightThird + 1);

		/* Then we suppose that the minimum value is in range [leftThird, right] */
		if (fLeft <= fRight)
		{
            left = leftThird;
		}
		else if (fRight > fLeft)
		{
			right = rightThird;
		}
    }

	return GetCost(DeliveryIngredients, DeliveryCount, Days, center, Money) + DeliveryFee * center;
}

int GetMaxDays(
	struct DeliveryItem DeliveryIngredients[],
	int DeliveryCount,
	int Money,
	int DeliveryFee) 
{
    int left = 0;
    int right = Money;
    int center = 0;

    while (left < right) 
	{
        int c = (left + right + 1) / 2;

		if (GetMinCost(
	         DeliveryIngredients,
	         DeliveryCount,
	         center, 
	         Money,
	         DeliveryFee) <= Money)
		{
			left = center;
		} 
		else 
		{
            right = center - 1;
        }
    }

    return left;
}

void Pr9Solve()
{
    int Money = 32;
	int DeliveryFee = 5;
	int N = 2;

    struct Offer Offers[100] = { { 5, 0 + 1 }, { 10, 2 + 1 } };
	int OffersCount = 2;

	struct DeliveryItem DeliveryIngredients[100];
    int DeliveryCount = 0;

    int i = 0;

    int DeliveryCost = 0;
    int Day = 0;
	int NumDaysFeed = 0;

	/* We have a definite sum of money: Money
	 * We can make one order of several orders.
	 * Lets describe one order or one purchase:
	 *  1. We sort all meals by price. We prefer cheaper ones
	 *     to more expensive.
	 *  2. Then we need to build an additional array:
	 *     [day, money_spent, price]
	 *  For example:
	 *     [0, 2,  1] 1
	 *     [2, 4,  2] 3
	 *     [6, 8,  3] 5
	 *     [8, 10, 2] 7
	 *  After making a purchase we have:
	 *  - In the day 0 we spent 2 sums of money for the meal 1
	 *    and we can live from day [0..2] to 2-nd day eating this type of meal
	 *  - In the day 2 we spent (4 - 2) sum of money for the meal 3
	 *    and we can live from day [2..6] to 6-th day eating this type of meal
	 *  - In the day 6 we spent (8 - 4) sum of money for the meal 5
	 *    and we can live from day [6..8] to 8-th day eating this type of meal
	 *  - In the day 8 we spent (10 - 8) sum of money for the meal 7
	 *    and 
	 */

	/* Sort all food by price. The cheapest ones 
	 * go in the begining
	 */
	Pr9HeapSort(Offers, 0, OffersCount);
    
	for (i = 0; i < N; i++) 
	{
		/* Last is the current day we order the food
		 * Time stale must be more than current day.
		 * Otherwise it is pointless 
		 */

		if (Offers[i].TimeToStale > Day) 
		{
			int DaysLeft = Offers[i].TimeToStale - Day;
			/* According to the condition we have that 
			 * eating one postion of some food that costs
			 * price_one_food we can live one day. 
			 * If we have a sum of money: SomeSum 
			 * and price: price_one_food. For this sum of 
			 * money we can buy num = SomeSum / price_one_food portions 
			 * of the food. That means we can live and eat this food 
			 * during the num days. 
			 */
			int MoneyLeft = Money - DeliveryCost;
			int PriceForOneFood = Offers[i].Price;
			int FoodDays = MoneyLeft / PriceForOneFood;
			/* Number of days the food can be fit for eating 
			 * since the last time. 
			 */
			int FoodDaysNotBecomeStale = Offers[i].TimeToStale - Day;
            /* The maximum number of days we can live eating i-th 
			 * type of food. This is the minimum of how many portions we 
			 * can buy and how many days the food will not be stale.
			 */
			NumDaysFeed = FoodDaysNotBecomeStale < FoodDays ? FoodDaysNotBecomeStale : FoodDays;
            /* max_added is a suplemental number of days we can leave
			 * If max_added is less than zero, our money has been over
			 * so we break the cycle
			 */
			if (NumDaysFeed <= 0) 
			{
                break;
            }
			/* min_meals is the day when we finish eating one food
			 * and start eating another one.
			 */
			DeliveryIngredients[DeliveryCount].Day = Day;
			/* start_cost is the sums of money has been
			 * spend for all previous meals
			 */
			DeliveryIngredients[DeliveryCount].Cost = DeliveryCost;
			/* price for the food
			 */
			DeliveryIngredients[DeliveryCount].Price = PriceForOneFood;

            DeliveryCount++;

            Day += NumDaysFeed;

            DeliveryCost += NumDaysFeed * PriceForOneFood;
		}
	}
    /* Supplemental information 
	 * This information is needed for a situation when we do not have enough money
	 */
    DeliveryIngredients[DeliveryCount].Day = Day + 1;
	DeliveryIngredients[DeliveryCount].Cost = Money + 1;
	DeliveryIngredients[DeliveryCount].Price = 1;

	GetMaxDays(DeliveryIngredients, DeliveryCount, Money, DeliveryFee);
}


