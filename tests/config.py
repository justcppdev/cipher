import pytest
import os


def get_cmake_config_type():
    try:
        return '-' + os.environ['CMAKE_CONFIG_TYPE']
    except KeyError:
        return ''


@pytest.fixture(scope='session', autouse=True)
def PATH():
    return {
        'cryptor':
            '_builds/default' + get_cmake_config_type() + '/sources/cryptor'
    }
