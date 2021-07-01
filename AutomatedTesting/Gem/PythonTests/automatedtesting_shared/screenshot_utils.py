"""
Copyright (c) Contributors to the Open 3D Engine Project. For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""

import os
import string

from .file_utils import move_file
from ly_test_tools.environment.waiter import wait_for
from ly_test_tools.image.screenshot_compare_qssim import qssim as compare_screenshots

from ly_remote_console.remote_console_commands import capture_screenshot_command as capture_screenshot_command
from ly_remote_console.remote_console_commands import send_command_and_expect_response as send_command_and_expect_response


def get_next_screenshot_at_path(screenshot_path, prefix='screenshot', num_digits=4):
    """
    :param screenshot_path: Root folder where the screenshots are being generated by the Launcher pr Editor.
    :param prefix: Generated screenshot files are named sequentially using the prefix.
                  e.g: screenshot0000.jpg, screenshot0001.jpg and so on. 
    :param num_digits: How many digits are used for file name formation.
    :return: A string with the file name (relative to screenshot_path).
    """
    max_counter = 10**num_digits
    counter = 0
    while counter < max_counter:
        numberstr = "{}".format(counter)
        formattednumber = numberstr.zfill(num_digits)
        filename = "{}{}.jpg".format(prefix, formattednumber)
        filepath = os.path.join(screenshot_path, filename)
        if not os.path.exists(filepath):
            #This filename is available.
            return filename
    raise AssertionError("All possible screenshot names at directory {} are taken".format(screenshot_path))


def take_screenshot(remote_console_instance, workspace, screenshot_name):
    """
    Takes an in game screenshot using the remote console instance passed in, validates that the screenshot exists
    and then renames that screenshot to something defined by the user of this function.
    :param remote_console_instance: Remote console instance that is attached to a specific launcher instance
    :param workspace: workspace instance so we can get the platform cache folder.
    :param screenshot_name: Name of the screenshot
    :return: None
    """
    screenshot_path = os.path.join(workspace.paths.platform_cache(), 'user', 'screenshots')
    expected_screenshot_name = get_next_screenshot_at_path(screenshot_path)
    capture_screenshot_command(remote_console_instance)
    wait_for(lambda: os.path.exists(os.path.join(screenshot_path, expected_screenshot_name)),
             timeout=10,
             exc=AssertionError('Screenshot at path:{} and with name:{} not found.'.format(screenshot_path, expected_screenshot_name)) )
    wait_for(lambda: rename_screenshot(screenshot_path, screenshot_name),
             timeout=10,
             exc=AssertionError('Screenshot at path:{} and with name:{} is still in use.'.format(screenshot_path, screenshot_name)))


def rename_screenshot(screenshot_path, screenshot_name):
    """
    Tries to rename the screenshot when the file is done being written to
    :param screenshot_path: Path to the Screenshot folder
    :param screenshot_name: Name we wish to change the screenshot to
    :return: True when operation is completed, False if the file is still in use
    """
    try:
        src_img = os.path.join(screenshot_path, 'screenshot0000.jpg')
        dst_img = os.path.join(screenshot_path, '{}.jpg'.format(screenshot_name))
        print('Trying to rename {} to {}'.format(src_img, dst_img))
        os.rename(src_img, dst_img)
        return True
    except Exception as e:
        print('Found error {0} when trying to rename screenshot.'.format(str(e)))
        return False


def move_screenshots(screenshot_path, file_type, logs_path):
    """
    Moves screenshots of a specific file type to the flume location so we can gather all of the screenshots we took.
    :param screenshot_path: Path to the screenshot folder
    :param file_type: Types of Files to look for.  IE .jpg, .tif, etc
    :param logs_path: Path where flume gathers logs to be upload
    """
    for file_name in os.listdir(screenshot_path):
        if file_name.endswith(file_type):
            move_file(screenshot_path, logs_path, file_name)


def move_screenshots_to_artifacts(screenshot_path, file_type, artifact_manager):
    """
    Saves screenshots of a specific file type to the artifact manager then removes the original files
    :param screenshot_path: Path to the screenshot folder
    :param file_type: Types of Files to look for.  IE .jpg, .tif, etc
    :param artifact_manager:  The artifact manager to save the artifacts to
    """
    for file_name in os.listdir(screenshot_path):
        if file_name.endswith(file_type):
            full_path_name = os.path.join(screenshot_path, file_name)
            artifact_manager.save_artifact(full_path_name)
            os.remove(full_path_name)


def compare_golden_image(similarity_threshold, screenshot, screenshot_path, golden_image_name,
                         golden_image_path=None):
    """
    This function assumes that your golden image filename contains the same base screenshot name and the word "golden"
    ex. pc_gamelobby_golden

    :param similarity_threshold: A float from 0.0 - 1.0 that determines how similar images must be or an asserts
    :param screenshot: A string that is the full name of the screenshot (ex. 'gamelobby_host.jpg')
    :param screenshot_path: A string that contains the path to the screenshots
    :param golden_image_path: A string that contains the path to the golden images, defaults to the screenshot_path
    :return:
    """
    if golden_image_path is None:
        golden_image_path = screenshot_path

    mean_similarity = compare_screenshots((os.path.join(screenshot_path, screenshot)),
                                          (os.path.join(golden_image_path, golden_image_name)))
    assert mean_similarity > similarity_threshold, \
        '{} screenshot comparison failed! Mean similarity value is: {}'\
        .format(screenshot, mean_similarity)


def download_qa_golden_images(project_name, destination_dir, platform):
    """
    Downloads the golden images for a specified project from s3. The project_name, platform, and filetype are used to
    filter which images will be downloaded as the golden images.

    https://s3.console.aws.amazon.com/s3/buckets/ly-qae-jenkins-configs/golden-images/?region=us-west-1&tab=overview

    :param project_name: a string of the project name of the folder in s3. ex: 'MultiplayerSample'
    :param destination_dir: a string of where the images will be downloaded to
    :param platform: a string for the platform type ('pc', 'android', 'ios', 'darwin', 'provo')
    :param filetype: a string for the file type. ex: '.jpg', '.png'
    :return:
    """

    # Currently we import s3_utils here instead of at the top because this is the only method that needs it,
    # and s3_utils has an unmet dependency on boto3 that hasn't been resolved.  Once s3_utils is functional again,
    # this can move back to the top of the file.
    try:
        from . import s3_utils as s3_utils
    except ImportError:
        raise Exception("Failed to import s3_utils")
    # end s3_utils import
        
    bucket_name = 'ly-qae-jenkins-configs'
    path = 'golden-images/{}/{}/'.format(project_name, platform)

    if not s3_utils.key_exists_in_bucket(bucket_name, path):
        raise s3_utils.KeyDoesNotExistError("Key '{}' does not exist in S3 bucket {}".format(path, bucket_name))
    for image in s3_utils.s3.Bucket(bucket_name).objects.filter(Prefix=path):
        file_name = string.replace(image.key, path, '')
        if file_name != '':
            s3_utils.download_from_bucket(bucket_name, image.key, destination_dir, file_name)


def _retry_command(remote_console_instance, command, output, tries=10, timeout=10):
        """
        Retries specified console command multiple times and asserts if it still can not send.
        :param remote_console: the remote console connected to the launcher.
        :param command: the command to send to the console.
        :param output: The expected output to check if the command was sent successfully.
        :param tries: The amount of times to try before asserting.
        :param timeout: The amount of time in seconds to wait for each retry send.
        :return: True if succeeded, will assert otherwise.
        """
        while tries > 0:
            tries -= 1
            try:
                send_command_and_expect_response(remote_console_instance, command, output)
                return True
            except:
                pass #Do nothing. Let the number of tries get to 0 if necessary.
        assert False, "Command \"{}\" failed to run in remote console.".format(command)


def prepare_for_screenshot_compare(remote_console_instance):
    """
    Prepares launcher for screenshot comparison. Removes any debug text and antialiasing that may result in interference
    with the comparison.

    :param remote_console_instance: Remote console instance that is attached to a specific launcher instance
    :return:
    """
    wait_for(lambda: _retry_command(remote_console_instance, 'r_displayinfo 0',
                                    '$3r_DisplayInfo = $60 $5[DUMPTODISK, RESTRICTEDMODE]$4'))
    wait_for(lambda: _retry_command(remote_console_instance, 'r_antialiasingmode 0',
                                    '$3r_AntialiasingMode = $60 $5[]$4'))
    wait_for(lambda: _retry_command(remote_console_instance, 'e_WaterOcean 0',
                                    '$3e_WaterOcean = $60 $5[]$4'))
