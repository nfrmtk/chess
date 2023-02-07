import pathlib

import pytest


import json


pytest_plugins = ['pytest_userver.plugins.redis']
# /// [redis setup]


# /// [service_env]
@pytest.fixture(scope='session')
def service_env(redis_sentinels):
    secdist_config = {
        'redis_settings': {
            'taxi-tmp': {
                'password': '',
                'sentinels': redis_sentinels,
                'shards': [{'name': 'mymaster'}],
            },
        },
    }

    return {'SECDIST_CONFIG': json.dumps(secdist_config)}
    # /// [service_env]
