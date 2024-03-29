# Intervals / Set-inversion
# Robotique/UE 4.1 - TD 02 - 2023/2024
## Range-only localization

A static robot located at $x = (0, 0)^T$ receives range-only (distance) signals from two landmarks located at $b1 = (1, 2)^T$ and $b2 = (3.6, 2.4)^T$. The observation equation yi = gi(x) is given by:
$yi = \sqrt{(xi1 - bi1)^2 + (xi2 - bi2)^2}$, i = 1, 2

We assume that two distances are received from the landmarks. Considering bounded uncertainties, we state
the actual observations are bounded as:
- $2 ≤ y1 ≤ 2.4$
- $4.1 ≤ y2 ≤ 4.5$

### 1. Show that this problem is a set-inversion problem by giving an equation of the set X of feasible positions of the robot.

$X = g1^{-1}([y1]) \cap g2^{-1}([y2]) = G^{-1}([Y])$

G(x) = $\begin{bmatrix} \sqrt{(x1 - 1)^2 + (x2 - 2)^2} \\ \sqrt{(x1 - 3.6)^2 + (x2 - 2.4)^2} \end{bmatrix}$

### 2. Considering only one landmark for now, compute the set X of feasible positions of the robot. The set X while be bounded by two subpavings using a SIVIA algorithm. The initial 2d box of feasible positions is $[x0] = [−10, 10]^2$

We have a green circle with yellow boundaries for the first landmark and a bleu background.

### 3. Verify that the actual position $x = (0, 0)^T$ is contained in the computed set.

We see that the point (0, 0) is in the set thanks to vibes.

![Vibes](/240328_TD2/vibes_q3.png)

### 4. Extend your set-inversion algorithm by considering two measurements instead of only one. Is the obtained computed set smaller than the previous one from Question 2?

We notice that only the two places where the cercles intersect are left.

So indeed, the set is smaller.

![Vibes](/240328_TD2/vibes_q4.png)

### 5. Add a third landmark $b(3) = (−4, −3)^T$, with its corresponding bounded measurement, and compute the resulting set X.

![Vibes](/240328_TD2/vibes_q5.png)

### 6. What would be X if we know that one of the three measurements is wrong, without knowing which value is wrong? Write the equation as for Question 1 and compute the resulting set.

We have to consider the three possible cases where one of the measurements is wrong.

This means that X could be the union of the three sets obtained by considering each of the three cases.

![Vibes](/240328_TD2/vibes_q6.png)