# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
import os
from pathlib import Path
from pixiv_crawel.items import PixivIllustItem
import urllib
from urllib import request

# import pudb
# pudb.set_trace()


def download_illust(illust_url, illust_referer):
    request_url = request.Request(url=illust_url, headers={
        "Referer": illust_referer})
    return urllib.request.urlopen(request_url).read()


class PixivIllustPipeline(object):
    # todo int the download part, get a setting like override
    # if override is set override the existing illust else not
    def process_item(self, item, spider):
        # check is item valid
        # check is member dirctory created
        save_path = item["save_path"] + "/" + item["member_id"]
        if(os.path.exists(save_path) is False):
            os.makedirs(save_path)

        if item["is_multi"] == 1:
            # handle multi image stiuation
            illust_path = save_path + "/" + item["illust_title"]
            if(os.path.exists(illust_path) is False):
                os.makedirs(illust_path)
            for illust_url in item["illust_urls"]:
                # illust name already stored as folder's name
                # save illust's download name

                temp_name = illust_url.split("/")
                my_file = Path(illust_path + "/" +
                               temp_name[len(temp_name) - 1])
                if(my_file.exists() is False):
                    f = open(illust_path + "/" +
                             temp_name[len(temp_name) - 1], "wb")
                    f.write(download_illust(illust_url, item["referer_url"]))
                    f.close()
        else:
            # some member name all their illust with the same name
            # todo for there is only one picture, just save with its title
            temp_name = item["illust_urls"].split("/")
            my_file = Path(save_path + "/" +
                           temp_name[len(temp_name) - 1])
            if my_file.exists() is False:
                f = open(save_path + "/" +
                         temp_name[len(temp_name) - 1],  # with extension
                         "wb")
                # todo handle error and use twisted
                f.write(download_illust(
                    item["illust_urls"], item["referer_url"]))
                f.close()
        return item
