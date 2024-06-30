from setuptools import setup, Extension

module = Extension(
    'dateutil',
    sources=['dateutil.c'],
    extra_compile_args=['-O3'],  # 编译时的优化选项
)

setup(
    name='dateutil',
    version='1.0',
    description='A C extension for date manipulation in Python',
    author='waitaza',
    author_email='3442235320@qq.com',
    url='https://github.com/waitaza/dateutil',
    ext_modules=[module],
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: C',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
    ],
    keywords='dateutil date manipulation C extension',
    license='MIT',
)
