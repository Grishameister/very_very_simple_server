# very_very_simple_server

Простой сервер с отдачей статики, можно отрефакторить код, конечно.

# Запуск сервера:
```
sudo docker build -t highload_server . && sudo docker run -p 8889:8889 highload_server
```
# Результаты функционального тестирования:
```
./httptest.py localhost 8889
test_directory_index (__main__.HttpServer)
directory index file exists ... ok
test_document_root_escaping (__main__.HttpServer)
document root escaping forbidden ... ok
test_empty_request (__main__.HttpServer)
Send empty line ... ok
test_file_in_nested_folders (__main__.HttpServer)
file located in nested folders ... ok
test_file_not_found (__main__.HttpServer)
absent file returns 404 ... ok
test_file_type_css (__main__.HttpServer)
Content-Type for .css ... ok
test_file_type_gif (__main__.HttpServer)
Content-Type for .gif ... ok
test_file_type_html (__main__.HttpServer)
Content-Type for .html ... ok
test_file_type_jpeg (__main__.HttpServer)
Content-Type for .jpeg ... ok
test_file_type_jpg (__main__.HttpServer)
Content-Type for .jpg ... ok
test_file_type_js (__main__.HttpServer)
Content-Type for .js ... ok
test_file_type_png (__main__.HttpServer)
Content-Type for .png ... ok
test_file_type_swf (__main__.HttpServer)
Content-Type for .swf ... ok
test_file_urlencoded (__main__.HttpServer)
urlencoded filename ... ok
test_file_with_dot_in_name (__main__.HttpServer)
file with two dots in name ... ok
test_file_with_query_string (__main__.HttpServer)
query string with get params ... ok
test_file_with_slash_after_filename (__main__.HttpServer)
slash after filename ... ok
test_file_with_spaces (__main__.HttpServer)
filename with spaces ... ok
test_head_method (__main__.HttpServer)
head method support ... ok
test_index_not_found (__main__.HttpServer)
directory index file absent ... ok
test_large_file (__main__.HttpServer)
large file downloaded correctly ... ok
test_post_method (__main__.HttpServer)
post method forbidden ... ok
test_request_without_two_newlines (__main__.HttpServer)
Send GET without to newlines ... ok
test_server_header (__main__.HttpServer)
Server header exists ... ok

----------------------------------------------------------------------
Ran 24 tests in 0.042s

OK

```

# Benchmark highload_server(in docker)

```
ab -c 100 -n 10000 http://localhost:8889/httptest/wikipedia_russia.html
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        highload_server
Server Hostname:        localhost
Server Port:            8889

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      100
Time taken for tests:   9.930 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9549170000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    1007.04 [#/sec] (mean)
Time per request:       99.301 [ms] (mean)
Time per request:       0.993 [ms] (mean, across all concurrent requests)
Transfer rate:          939101.49 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    2   0.6      2       9
Processing:    19   98  11.5     97     236
Waiting:        1    5  11.6      2     142
Total:         21   99  11.5     99     237

Percentage of the requests served within a certain time (ms)
  50%     99
  66%    100
  75%    101
  80%    101
  90%    103
  95%    106
  98%    115
  99%    130
 100%    237 (longest request)

```

# Benchmark nginx(without docker and sendfile on)
```
ab -c 100 -n 10000 http://localhost:8080/httptest/wikipedia_russia.html
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.14.0
Server Hostname:        localhost
Server Port:            8080

Document Path:          /httptest/wikipedia_russia.html
Document Length:        954824 bytes

Concurrency Level:      100
Time taken for tests:   8.635 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      9550710000 bytes
HTML transferred:       9548240000 bytes
Requests per second:    1158.09 [#/sec] (mean)
Time per request:       86.349 [ms] (mean)
Time per request:       0.863 [ms] (mean, across all concurrent requests)
Transfer rate:          1080132.62 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        1    1   0.4      1       7
Processing:    18   85   8.3     83     141
Waiting:        1    2   4.5      2      70
Total:         18   86   8.4     84     144

Percentage of the requests served within a certain time (ms)
  50%     84
  66%     86
  75%     87
  80%     88
  90%    100
  95%    105
  98%    110
  99%    112
 100%    144 (longest request)
```
