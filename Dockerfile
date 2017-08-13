FROM ruby:2.4.1

MAINTAINER Kouhei Sutou <kou@clear-code.com>

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install
