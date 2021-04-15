"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""

import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import azlmbr.editor as editor
import azlmbr.legacy.general as general
import azlmbr.bus as bus
import azlmbr.math as math
import azlmbr.paths
import azlmbr.vegetation as vegetation

sys.path.append(os.path.join(azlmbr.paths.devroot, 'AutomatedTesting', 'Gem', 'PythonTests'))
import automatedtesting_shared.hydra_editor_utils as hydra
from automatedtesting_shared.editor_test_helper import EditorTestHelper
from largeworlds.large_worlds_utils import editor_dynveg_test_helper as dynveg


class TestInstanceSpawnerPriority(EditorTestHelper):
    def __init__(self):
        EditorTestHelper.__init__(self, log_prefix="InstanceSpawnerPriority", args=["level"])

    def run_test(self):
        """
        Summary:
        A new level is created. An instance spawner area and blocker area are setup to overlap. Instance counts are
        verified with the initial setup. Layer priority on the blocker area is set to lower than the instance spawner
        area, and instance counts are re-verified.

        Expected Behavior:
        Vegetation areas with a higher Layer Priority plant over those with a lower Layer Priority

        Test Steps:
         1) Create a new level
         2) Create overlapping instance spawner and blocker areas
         3) Create a surface to plant on
         4) Validate initial instance counts in the spawner area
         5) Reduce the Layer Priority of the blocker area
         6) Validate instance counts in the spawner area
         
        Note:
        - This test file must be called from the Open 3D Engine Editor command terminal
        - Any passed and failed tests are written to the Editor.log file.
                Parsing the file or running a log_monitor are required to observe the test results.

        :return: None
        """

        # 1) Create a new, temporary level
        self.test_success = self.create_level(
            self.args["level"],
            heightmap_resolution=1024,
            heightmap_meters_per_pixel=1,
            terrain_texture_resolution=4096,
            use_terrain=False,
        )

        # Set view of planting area for visual debugging
        general.set_current_view_position(512.0, 500.0, 38.0)
        general.set_current_view_rotation(-20.0, 0.0, 0.0)

        # 2) Create overlapping areas: 1 instance spawner area, and 1 blocker area
        spawner_center_point = math.Vector3(508.0, 508.0, 32.0)
        asset_path = os.path.join("Slices", "PinkFlower.dynamicslice")
        spawner_entity = dynveg.create_vegetation_area("Instance Spawner", spawner_center_point, 16.0, 16.0, 1.0,
                                                       asset_path)
        blocker_center_point = math.Vector3(516.0, 516.0, 32.0)
        blocker_entity = dynveg.create_blocker_area("Instance Blocker", blocker_center_point, 16.0, 16.0, 1.0)

        # 3) Create a surface for planting
        planting_surface_center_point = math.Vector3(512.0, 512.0, 32.0)
        dynveg.create_surface_entity("Planting Surface", planting_surface_center_point, 64.0, 64.0, 1.0)

        # Set instances to spawn on a center snap point to avoid unexpected instances around the edges of the box shape
        veg_system_settings_component = hydra.add_level_component("Vegetation System Settings")
        editor.EditorComponentAPIBus(bus.Broadcast, "SetComponentProperty", veg_system_settings_component,
                                     'Configuration|Area System Settings|Sector Point Snap Mode', 1)

        # 4) Validate the expected instance count with initial setup. GetAreaProductCount is used as
        # GetInstanceCountInAabb does not filter out blocked instances
        num_expected = (20 * 20) - (10 * 10)  # 20 instances per 16m per side minus 1 blocked quadrant
        result = self.wait_for_condition(lambda: dynveg.validate_instance_count_in_entity_shape(spawner_entity.id,
                                                                                                num_expected), 5.0)
        self.test_success = self.test_success and result

        # 5) Change the Instance Spawner area to a higher layer priority than the Instance Blocker
        blocker_entity.get_set_test(0, 'Configuration|Layer Priority', 0)

        # 6) Validate the expected instance count with changed area priorities
        num_expected = 20 * 20  # 20 instances per 16m per side, no instances should be blocked at this point
        result = self.wait_for_condition(lambda: dynveg.validate_instance_count_in_entity_shape(spawner_entity.id,
                                                                                                num_expected), 5.0)
        self.test_success = self.test_success and result

        # 7) Revert Layer Priority changes so both areas are equal, and change Sub Priority to a higher value on the
        # Instance Spawner area
        blocker_entity.get_set_test(0, 'Configuration|Layer Priority', 1)
        spawner_entity.get_set_test(0, 'Configuration|Sub Priority', 100)
        blocker_entity.get_set_test(0, 'Configuration|Sub Priority', 1)

        # 8) Validate the expected instance count with changed area priorities
        num_expected = 20 * 20  # 20 instances per 16m per side, no instances should be blocked at this point
        result = self.wait_for_condition(lambda: dynveg.validate_instance_count_in_entity_shape(spawner_entity.id,
                                                                                                num_expected), 5.0)
        self.test_success = self.test_success and result


test = TestInstanceSpawnerPriority()
test.run()
