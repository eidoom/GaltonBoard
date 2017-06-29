import numpy as np

class Barrier(object):
    def __init__(self, min_x, max_x, fn):
        self.min = min_x
        self.max = max_x

        self.fun = np.vectorize(fn)
        #self.fun = lambda x: np.piecewise(x, 
        #                        [x < self.min, (x >= self.min) & (x < 0), (x >= 0) & (x < self.max), x >= self.max],
        #                        [np.inf, fn, lambda x: -1.5*x-1.54, np.inf]
        #            )

    def check(self, x):
        """Return bool array selecting the Nx2 grid elements that touch the line."""
        return (
            (x[:,0] > self.min) 
            & (x[:,0] < self.max) 
            & (np.abs(self.fun(x[:,0]) - x[:,1]) < 0.1)
        )

    def norm(self, x):
        m = (self.fun(x+0.001) - self.fun(x-0.001)) / 0.002
        return np.array([-m,1])

    @property
    def xs(self):
        return np.linspace(self.min, self.max, 60)

    @property
    def ys(self):
        return self.fun(self.xs)


preset_barriers = {

'empty' : 
[
Barrier(-2.5, -0.2, lambda x: 1.5*x+1.75),
Barrier(0.2, 2.5 , lambda x: -1.5*x+1.75),
],


'triangle' : [
Barrier(-2.5, -0.2, lambda x: 1.5*x+1.75),
Barrier(-0.66, 0.66, lambda x: -1.5*np.abs(x)-0.34),
Barrier(0.2, 2.5 , lambda x: -1.5*x+1.75),
], #(-2, -0.2, lambda x: 1.45*x + 1.45)


'funnel' : [
Barrier(-2.5, -0.2, lambda x: 1.5*x+1.75),
Barrier(-0.66, -0.05, lambda x: 1.5*x-0.34),
Barrier(0.05, 0.66, lambda x: -1.5*x-0.34),
Barrier(0.2, 2.5 , lambda x: -1.5*x+1.75),
], #(-2, -0.2, lambda x: 1.45*x + 1.45)


'left' : [
Barrier(-2.5, -0.2, lambda x: 1.5*x+1.75),
Barrier(-0.66, 0.66, lambda x: -1.5*np.abs(x)-0.34),
Barrier(0.2, 2.5 , lambda x: -1.5*x+1.75),
], #(-2, -0.2, lambda x: 1.45*x + 1.45)


'right' : [
Barrier(-2.5, -0.2, lambda x: 1.5*x+1.75),
Barrier(-0.66, 0.66, lambda x: -1.5*np.abs(x)-0.34),
Barrier(0.2, 2.5 , lambda x: -1.5*x+1.75),
], #(-2, -0.2, lambda x: 1.45*x + 1.45)


}
#barr = [
#Barrier(-0.5,  0.5, lambda x: 1.5*x-1.54),
#Barrier(-1.5, -0.5, lambda x: 1.5*x+0.15),
#]


if __name__ == '__main__':
    barrier = Barrier(-5, 5, lambda x: -x)
    x = np.random.uniform(-1,1,size=(20,2))
    print x
    print barrier.fun(x[:,0])

