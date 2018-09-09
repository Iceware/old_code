import scrapy
import os
import logging
from scrapy.utils.project import get_project_settings
from scrapy.selector import Selector
from scrapy.http import HtmlResponse
from pixiv_crawel.items import PixivIllustItem


class StartSpider(scrapy.Spider):
    name = "member_spider"
    cookie = {}
    settings = get_project_settings()

    def read_cookie(self):
        """
        return the cookie dict
        """
        result = {}
        pwd = os.path.dirname(os.path.abspath(__file__))

        with open(pwd + "/../../cookie2", "r") as f:
            lines = f.readlines()

            for line in lines:
                if len(line) < 2 or line[1] == " ":
                    continue
                else:
                    slices = line.split("\t")
                    # print(slices)
                    result[slices[5]] = slices[6]
            f.close()
        return result

    def start_requests(self):
        self.cookie = self.read_cookie()
        # todo read id from setting
        ids=["1"]

        for member_id in ids:
            yield scrapy.Request(url=self.settings.get("MEMBER_URL_BASE") + member_id,
                                 cookies=self.cookie, callback=self.parse_member_page)

    def parse_member_page(self, response):
        """
        yeild every illust in the current page
        yeild next page
        """

        illust_urls = response.selector.xpath(
            "//ul[@class='_image-items']/li/a[contains(@class,'work')]/@href").extract()
        for url in illust_urls:
            yield scrapy.Request(url=self.settings.get("PIXIV_URL") + url,
                                 cookies=self.cookie, callback=self.pase_illust_page)
        # currently this yeild all page url to let dupilifilter to handle
        # todo yeild remain pages url
        work_page_urls = response.selector.xpath(
            "//ul[@class='page-list']/li/a/@href").extract()
        for work_page_url in work_page_urls:
            yield scrapy.Request(url=self.settings.get("MEMBER_URL_BASE_NID") + work_page_url,
                                 cookies=self.cookie, callback=self.parse_member_page)

    def pase_illust_page(self, response):
        """
        yeild the picture source page to download
        """
        member_url = response.selector.xpath(
            "//a[contains(@class,'user-name')]/@href")
        if(len(member_url) < 0):
            logging.log(logging.WARNING,
                        "error in read illust user id" + response.url)
            return
        # get the member id and illust title
        member_id = member_url.extract_first().split("=")[1]
        illust_title = response.selector.xpath(
            "//h1[@class='TTmQ_bQ']/text()").extract_first()
        referer_url = response.url
        # first check whether has multiple picture
        src_urls = response.selector.xpath(
            "//a[contains(@class,'_2t-hEST')]/@href").extract_first()
        if src_urls is not None:
            return scrapy.Request(url=self.settings.get("PIXIV_URL") + src_urls,
                                  cookies=self.cookie, headers={
                                      "Referer": referer_url},
                                  callback=self.pase_illusts_page,
                                  meta={"member_id": member_id,  # pass these value to next parser may need to change
                                        "illust_title": illust_title,
                                        "referer_url": referer_url,
                                        "src_urls": src_urls})
        else:
            # only has single picture
            src_url = response.selector.xpath(
                "//img[@class='item-container']/@data-src").extract_first()

            # create the item to download
            item = PixivIllustItem()
            item["member_id"] = member_id
            item["illust_title"] = illust_title
            item["illust_urls"] = src_url
            item["referer_url"] = referer_url
            item["save_path"] = self.settings.get("IMAGE_STORAGE")
            item["is_multi"] = 0
            return item

    def pase_illusts_page(self, response):
        # at this time only one img is present in the div
        meta = response.meta
        # unlike single illust the src url is in data-src arttibute
        illust_urls = response.selector.xpath(
            "//div[@class='item-container']/img/@data-src").extract()
        item = PixivIllustItem()
        item["member_id"] = meta["member_id"]
        item["illust_title"] = meta["illust_title"]
        item["illust_urls"] = illust_urls
        item["referer_url"] = meta["referer_url"]
        item["save_path"] = self.settings.get("IMAGE_STORAGE")
        item["is_multi"] = 1
        return item
