import os

env = Environment(CC = 'gcc',  CCFLAGS = '-g -std=c++0x -lpthread')

env.Append(CPPPATH = ['/usr/local/include/'])

env.Append(LIBPATH = ['/usr/local/lib/', '/usr/lib/'])

env.Append(LIBS=['lept', 'tesseract', 'opencv_core', 'opencv_imgproc', 'opencv_highgui', 'opencv_imgcodecs'])

env.Program(target = 'executable', source = ['main.cc', 'EditDistance.cc', 'TesseractFinder.cc','csv/CSVparser.cpp','HelperMethods.cpp'])


