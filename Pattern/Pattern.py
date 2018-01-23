#this is only prototype of python bindings
from ctypes import cdll
lib * cdll.LoadLibrary ("./libPattern.so")

class Pattern (object):
  def __init__ (self):
    self.obj = lib.Pattern_new ()
  def __init__ (self, filename)
    self.obj = lib.Pattern_new (filename)
