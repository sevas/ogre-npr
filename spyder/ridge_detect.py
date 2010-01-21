# -*- coding: utf-8 -*-
"""
Created on Sun Oct 18 16:13:26 2009

@author: sevas
"""

import math as m

def normalize(v):
    norm = m.sqrt(sum([c*c for c in v]))
    return [float(c)/norm for c in v]
    
def rad2deg(val):
    return val * (180./m.pi)


def deg2rad(val):
    return val * (m.pi/180)


def isRidge(a, b, thresh):
    dotp = np.dot(a, b) 
    print "ab angle : %3f    (%.1f)" % (m.acos(dotp), rad2deg(m.acos(dotp)))
    print dotp, m.cos(deg2rad(float(thresh)))
    return dotp < m.cos(deg2rad(float(thresh)))






if __name__=="__main__":
    nA = np.array([126, 417, 123])
    nB = np.array([-126, 417, -123])


    thresh_angle = 60.0

    nA, nB = normalize(nA), normalize(nB)
    print isRidge(nA, nB, thresh_angle)

    a = np.array([1, 0, 0])
    b = np.array([1.0/m.sqrt(2), 0, 1.0/m.sqrt(2)])
    print isRidge(b, a, thresh_angle)
    
    a, b = np.array([1, 0, 0]), np.array([0, 0, 1])
    print isRidge(b, a, thresh_angle)

    
