from setuptools import setup, find_packages, Extension
import glob
import os.path
import sys


C_SRC_PREFIX = os.path.join('cbits', 'webrtcaec')

c_sources = (
    [os.path.join('cbits', 'webrtcaec.c')]
    + glob.glob(os.path.join(C_SRC_PREFIX,'*.c')))

define_macros = []

if sys.platform.startswith('win'):
    define_macros.extend([('_WIN32', None),])
else:
    define_macros.extend([('WEBRTC_POSIX', None),])

module = Extension('_webrtcaec',
                   define_macros=define_macros,
                   sources=c_sources,
                   include_dirs=['cbits'])

here = os.path.abspath(os.path.dirname(__file__))

# Get the long description from the README file
with open(os.path.join(here, 'README.md')) as f:
    long_description = f.read()

setup(
    name='webrtcaec',
    author='Chang Sun',
    author_email='2721988848@qq.com',
    version='1.0.7',
    description=('Python interface to the webrtc aec'),
    long_description=long_description,
    url='***',
    classifiers=[
        # How mature is this project? Common values are
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 4 - Beta',

        # Indicate who your project is intended for
        'Intended Audience :: Developers',
        'Topic :: Scientific/Engineering :: Artificial Intelligence',
        'Topic :: Scientific/Engineering :: Information Analysis',
        'Topic :: Scientific/Engineering :: Human Machine Interfaces',

        # Pick your license as you wish (should match "license" above)

        # Specify the Python versions you support here. In particular,
        # ensure that you indicate whether you support Python 2,
        # Python 3 or both.
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.2',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
    ],
    keywords='noise suppression speech enhancement denoise aec',
    ext_modules=[module],
    py_modules=['webrtcaec'])
