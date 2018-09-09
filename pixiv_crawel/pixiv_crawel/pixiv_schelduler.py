import os
import logging
import logging.handlers

LOG_FILE_NAME = "pixiv_scheduler.log"

logger = logging.getLogger(__name__)


class Scheduler(object):
    def __init__(self, server
                 ):
        pass

    # created from setting
    @classmethod
    def from_settings(cls, settings):
        pass

    @classmethod
    def from_crawler(cls, crawler):
        pass

    def __len__(self):
        pass
