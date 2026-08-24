import os
from glob import glob

from setuptools import find_packages, setup

package_name = 'nail_skill'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='jje',
    maintainer_email='jje320594@gmail.com',
    description='robot_skill_node: A계층 스킬 프리미티브 (NIS §5.1 / SDS §6.1)',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'robot_skill_node = nail_skill.robot_skill_node:main',
            'tool_manager = nail_skill.tool_manager_node:main',
            'probe_check = nail_skill.probe_check:main',
        ],
    },
)
