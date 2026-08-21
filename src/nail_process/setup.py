from setuptools import find_packages, setup

package_name = 'nail_process'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='jje',
    maintainer_email='jje320594@gmail.com',
    description='B계층 공정 노드 (NIS §6)',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'sanding_node = nail_process.sanding_node:main',
            'brushing_node = nail_process.brushing_node:main',
            'coating_node = nail_process.coating_node:main',
        ],
    },
)
