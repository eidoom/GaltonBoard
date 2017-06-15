import numpy as np

class Barrier(object):
    def __init__(self, min_x, max_x, fn):
        self.min = min_x
        self.max = max_x

        self.fun = np.vectorize(fn)
        # self.fun = lambda x: np.piecewise(x, 
        #                        [x < self.min, (x >= self.min) & (x < self.max), x >= self.max],
        #                        [np.inf, fn, np.inf]
        #            )

        self.norm = np.array([-1,1])

    def check(self, x):
        """Return bool array selecting the Nx2 grid elements that touch the line."""
        return (
            (x[:,0] > self.min) 
            & (x[:,0] < self.max) 
            & (np.abs(self.fun(x[:,0]) - x[:,1]) < 0.1)
        )

    @property
    def xs(self):
        return np.linspace(self.min, self.max, 30)

    @property
    def ys(self):
        return self.fun(self.xs)







if __name__ == '__main__':
    barrier = Barrier(-5, 5, lambda x: -x)
    x = np.random.uniform(-1,1,size=(20,2))
    print x
    print barrier.fun(x[:,0])

