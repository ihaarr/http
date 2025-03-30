#include <gtest/gtest.h>
#include <http/request/request.hpp>

TEST(REQUEST, PARSER) {
    auto data = 
    "POST /api/users?asd=asd HTTP/1.1\r\n"
    "Content-Type: application/json\r\n"
    "Authorization: Bearer <token>\r\n"
    "Accept: application/json\r\n"
    "\r\n"
    "{"
    "  \"name\": \"John Doe\","
    "  \"age\": 30"
    "}";
    auto res = http::request::Request::from_data(data);
    EXPECT_EQ(res.has_value(),true);
    auto v = res.value();
    EXPECT_EQ(v.method(), http::Method::POST);
    auto uri = "/api/users";
    auto query = "?asd=asd";
    auto url = http::Url{uri, query};
    EXPECT_TRUE(v.url() == url);
    EXPECT_EQ(v.header("Content-Type").get(), "application/json");
    EXPECT_EQ(v.header("Authorization").get(), "Bearer <token>");
    EXPECT_EQ(v.header("Accept").get(), "application/json");
    auto body = "{"
    "  \"name\": \"John Doe\","
    "  \"age\": 30"
    "}";
    auto bview = std::string_view(body);
    auto b = std::vector<http::byte>{bview.begin(), bview.end()};
    EXPECT_TRUE(std::equal(v.body().begin(), v.body().end(), b.begin()));
}