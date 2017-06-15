
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.collections import PatchCollection
from matplotlib.widgets import Button

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.1)

grid = np.mgrid[0.1:0.9:5j]
ecopt = 'none'
fcopt = 'black'

mys = mpatches.ArrowStyle.Simple(head_length=0.1, head_width=0.0, tail_width=0.25)

opt0 = mpatches.Circle((0.5, 0.5), 0.15, ec=ecopt, fc=fcopt)

opt1 = mpatches.RegularPolygon((0.5, 0.5), 3, 0.2, ec=ecopt, fc=fcopt)

opt2a = mpatches.Circle((0.3, 0.5), 0.1, ec=ecopt, fc=fcopt)
opt2b = mpatches.Circle((0.7, 0.5), 0.1, ec=ecopt, fc=fcopt)

opt3a = mpatches.FancyArrowPatch((0.05,0.65), (0.35,0.35), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)
opt3b = mpatches.FancyArrowPatch((0.35,0.65), (0.65,0.35), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)
opt3c = mpatches.FancyArrowPatch((0.65,0.65), (0.95,0.35), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)

opt4a = mpatches.FancyArrowPatch((0.05,0.35), (0.35,0.65), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)
opt4b = mpatches.FancyArrowPatch((0.35,0.35), (0.65,0.65), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)
opt4c = mpatches.FancyArrowPatch((0.65,0.35), (0.95,0.65), arrowstyle=mys, mutation_scale=40, ec=ecopt, fc=fcopt)

ax.add_patch(opt0)
ax.add_patch(opt1)
ax.add_patch(opt2a)
ax.add_patch(opt2b)
ax.add_patch(opt3a)
ax.add_patch(opt3b)
ax.add_patch(opt3c)
ax.add_patch(opt4a)
ax.add_patch(opt4b)
ax.add_patch(opt4c)

plt.axis('equal')
plt.axis('off')
ax.set_xlim([0,1])
ax.set_ylim([0,1])
ax.autoscale(False)

def allfalse():
    opt0.set_visible(False)
    opt1.set_visible(False)
    opt2a.set_visible(False)
    opt2b.set_visible(False)
    opt3a.set_visible(False)
    opt3b.set_visible(False)
    opt3c.set_visible(False)
    opt4a.set_visible(False)
    opt4b.set_visible(False)
    opt4c.set_visible(False)

allfalse()

#####buttons

def fun0(event):
    allfalse()
    opt0.set_visible(True)
    plt.draw()
ax0 = plt.axes([grid[0]-0.05, 0.05, 0.1, 0.1], aspect=1)
bopt0 = mpatches.Circle((0.5, 0.5), 0.35, ec=ecopt, fc=fcopt)
ax0.add_patch(bopt0)
plt.axis('off')
bb0 = Button(ax0,'')
bb0.on_clicked(fun0)

def fun1(event):
    allfalse()
    opt1.set_visible(True)
    plt.draw()
ax1 = plt.axes([grid[1]-0.05, 0.05, 0.1, 0.1], aspect=1)
bopt1 = mpatches.RegularPolygon((0.5, 0.5), 3, 0.5, ec=ecopt, fc=fcopt)
ax1.add_patch(bopt1)
plt.axis('off')
bb1 = Button(ax1,'')
bb1.on_clicked(fun1)

def fun2(event):
    allfalse()
    opt2a.set_visible(True)
    opt2b.set_visible(True)
    plt.draw()
ax2 = plt.axes([grid[2]-0.05, 0.05, 0.1, 0.1], aspect=1)
bopt2a = mpatches.Circle((0.2, 0.5), 0.2, ec=ecopt, fc=fcopt)
bopt2b = mpatches.Circle((0.8, 0.5), 0.2, ec=ecopt, fc=fcopt)
ax2.add_patch(bopt2a)
ax2.add_patch(bopt2b)
plt.axis('off')
bb2 = Button(ax2,'')
bb2.on_clicked(fun2)

def fun3(event):
    allfalse()
    opt3a.set_visible(True)
    opt3b.set_visible(True)
    opt3c.set_visible(True)
    plt.draw()
ax3 = plt.axes([grid[3]-0.05, 0.05, 0.1, 0.1], aspect=1)
bopt3 = mpatches.FancyArrowPatch((0,1), (1,0), arrowstyle=mys, mutation_scale=20, ec=ecopt, fc=fcopt)
ax3.add_patch(bopt3)
plt.axis('off')
bb3 = Button(ax3,'')
bb3.on_clicked(fun3)

def fun4(event):
    allfalse()
    opt4a.set_visible(True)
    opt4b.set_visible(True)
    opt4c.set_visible(True)
    plt.draw()
ax4 = plt.axes([grid[4]-0.05, 0.05, 0.1, 0.1], aspect=1)
bopt4 = mpatches.FancyArrowPatch((0,0), (1,1), arrowstyle=mys, mutation_scale=20, ec=ecopt, fc=fcopt)
ax4.add_patch(bopt4)
plt.axis('off')
bb4 = Button(ax4,'')
bb4.on_clicked(fun4)


plt.show()
