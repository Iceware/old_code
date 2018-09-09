# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class PixivIllustItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    member_id = scrapy.Field()
    illust_title = scrapy.Field()
    illust_urls = scrapy.Field()
    referer_url = scrapy.Field()
    save_path = scrapy.Field()
    is_multi = scrapy.Field()
    pass
