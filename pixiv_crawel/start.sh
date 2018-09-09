#!/bin/bash
spider_name=member_spider
pixiv_id=
password=
if [ "$1" == "login" ]
then
  echo "logging with pixiv_id",pixiv_id
  headers="user-agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/64.0.3282.167 Chrome/64.0.3282.167 Safari/537.36"
  curl -c cookie1 -H "$headers" "https://accounts.pixiv.net/login" > a.html
  post_key=`more a.html | grep -Eo 'name="post_key" value="(.*)"'|cut -d'"' -f 4`
  echo "post_key $post_key"
  curl --cookie cookie1 -c cookie2 -H "$headers" -d "pixiv_id=$pixiv_id&captcha=&g_recaptcha_response=&password=$password&post_key=$post_key&source=pc" "https://accounts.pixiv.net/api/login?lang=en" > b.html
fi
scrapy crawl $spider_name
