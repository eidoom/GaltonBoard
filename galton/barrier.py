import numpy as np

class Barrier(object):
    """A barrier that balls can interact with"""
    def __init__(self, min_x, max_x, fn):
        self.min = min_x
        self.max = max_x

        self.fun = np.vectorize(fn)
        #self.fun = lambda x: np.piecewise(x, 
        #                        [x < self.min, (x >= self.min) & (x < 0), (x >= 0) & (x < self.max), x >= self.max],
        #                        [np.inf, fn, lambda x: -m*x-1.54, np.inf]
        #            )

    def check(self, x):
        """Return bool array selecting the Nx2 grid elements that touch the line."""
        return (
            (x[:,0] > self.min) 
            & (x[:,0] < self.max) 
            & (np.abs(self.fun(x[:,0]) - x[:,1]) < 0.12)
        )

    def norm(self, x):
        """Crude approximation to the normal vector. Won't work at edges"""
        h = 0.001
        m = (self.fun(x+h) - self.fun(x-h)) / (2*h)
        return np.array([-m,1])

    @property
    def xs(self):
        return np.linspace(self.min, self.max, 5)

    @property
    def ys(self):
        return self.fun(self.xs)

# common slope of barriers
m = 1.5

L_WALL = Barrier(-2.5, -0.2, lambda x: m*x+1.67)
R_WALL = Barrier(0.2, 2.5 , lambda x: -m*x+1.67)

preset_barriers = {

'empty' :
[
L_WALL,
R_WALL,
],


'triangle' :
[
L_WALL,
Barrier(-0.66, 0.66, lambda x: -1.5*np.abs(x)-0.34),
R_WALL,
],


'funnel' : 
[
L_WALL,
Barrier(-1.27, -0.55, lambda x: -m*x-2.18),
Barrier(0.55, 1.27, lambda x: m*x-2.18),
R_WALL,
],


'left' : 
[
L_WALL,
Barrier(-1.4, -0.75, lambda x: m*x+0.7),
Barrier(-0.62, -0.02, lambda x: m*x-0.45),
Barrier(0.12, 0.75, lambda x: m*x-1.58),
R_WALL,
],


'right' : 
[
L_WALL,
Barrier(0.75, 1.4, lambda x: -m*x+0.7),
Barrier(0.02, 0.62, lambda x: -m*x-0.45),
Barrier(-0.75, -0.12, lambda x: -m*x-1.58),
R_WALL,
],


}

if __name__ == '__main__':
    barrier = Barrier(-5, 5, lambda x: -x)
    x = np.random.uniform(-1,1,size=(20,2))
    print x
    print barrier.fun(x[:,0])

