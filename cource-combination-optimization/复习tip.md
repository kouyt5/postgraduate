# 组合优化复习

## 1.线性规划以及单纯形法

+ 基向量与非基向量
+ 基解
+ 基可行解：$X_B \geq 0$
+ 非退化的解：在LP问题的一个基可行解中，如果它的所有的基变量取值大于0；如果有一个基变量取零值，则称它是退化的解
## 2.对偶单纯形法
1. 化为标准型，同时$b$化为负值
2. 换出：b最小的元素对应的列
3. 换入：$c/a$ 中最小值（a<0）

+ 定理1 对偶问题$(D)$的对偶就是原问题$(P)$。
$$Max \  Z=CX\Rightarrow Min\ W=Yb$$

$$\begin{cases}
        AX & \le b \\
        X & \geq 0 
        \end{cases}\Rightarrow \begin{cases}
        YA & \geq C \\
        Y & \geq 0 
        \end{cases}$$
+  定理2(弱对偶问题)：若$X$和$Y$分别为原问题$(P)$及其对偶问题$D$的任意可行解，则有$CX\le Yb$成立
   +  推论：若$X$和$Y$分别为原问题$(P)$及其对偶问题$(D)$的一对可行解, 则问题$(P)$及问题$(D)$都有最优解
   + 推论2：若原问题$(P)$有可行解，但无有限最优解，则对偶问题$(D)$无可行解
+ 定理3: 若$X^*$和$Y^*$分别是原始问题和对偶问题的可行解，且有$CX^*=Y^*b$，则$X^*$和$Y^*$分别是原始问题和对偶问题的最优解
+ 定理 4：(强对偶定理) 如果原问题$(P)$有最优解，那么对偶问题$(D)$也有最优解，且目标函数值相等。
+ 定理5：若原始问题的最优解存在，则用单纯形法求解时，其对偶问题的最优解可同时在最优单纯形表上得到，且顺次等于松弛变量或剩余变量对应的检验数的相反数
+ 定理6（互补松弛性）若$X^*$和$Y^*$分别是原始问题和对偶问题的可行解，且$X_s$和$Y_s$分别为它们的松弛变量和剩余变量，则 $Y^*X_s=0$和$Y_sX^*=0$当且仅当分别为它们的最优解

原问题与对偶问题的关系，有且仅有下列三种：
1. 两个都有最优解，且目标函数最优值相等；
2. 两个都无可行解；
3. 一个问题无界，则另一问题无可行解。

## 3.整数规划
+ 割平面法
+ 分支定界法
## 4.动态规划

+ 动态规划
+ 资源分配问题

## 5.背包问题

+ 0/1 背包问题的分支定界法
$$U^0=\sum_{j=0}^{s-1}p_j+\left \lfloor \bar{C}\frac{p_{s+1}}{w_{s+1}} \right \rfloor $$
$$U^1=\sum_{j=0}^{s-1}p_j+\left \lfloor p_s-(w_s-\bar{C})\frac{p_{s+1}}{w_{s+1}} \right \rfloor $$
$$U_1=\sum_{j=0}^{s-1}p_j+\left \lfloor \bar{C}\frac{p_{s}}{w_{s}} \right \rfloor $$
$$上界：\bold{U_2=max(U^0,U^1)\leq U_1}$$

+ 背包问题近似解法
$$1.GA_0 贪婪 $$
$$ 2.GA_1 取 max\{ \sum_{j=1}^{s-1}p_j,p_s\}（向前看一步）$$
$$ \ 3.GA_2: \max_j{p_j}（向前看n步）$$
## 6.指派问题
+ 求效率最高，即**最小值**
## 8.作业调度

+ 单机调度问题
  + $1||C_{max}$损失
  + $1||L_{max}$最大延误,最早工期优先
  + $1||U_{j}$
+ 平行机调度问题:$P_m||C_{max}$
  + LS
  + LPT
+ 车间作业调度问题