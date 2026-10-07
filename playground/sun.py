#!/usr/bin/env python3
"""Deneme alanını bu bilgisayarda sunar.

Kullanım: python playground/sun.py [port] [klasör]

Python'un hazır sunucusu (python -m http.server) bazı Windows kurulumlarında
.js dosyalarını yanlış içerik türüyle gönderir ve tarayıcı yorumlayıcıyı
yüklemeyi reddeder; bu betik içerik türlerini açıkça belirler.
"""
import http.server
import os
import sys

port = int(sys.argv[1]) if len(sys.argv) > 1 else 8000
directory = sys.argv[2] if len(sys.argv) > 2 else os.path.dirname(os.path.abspath(__file__))


class Handler(http.server.SimpleHTTPRequestHandler):
    extensions_map = {
        **http.server.SimpleHTTPRequestHandler.extensions_map,
        ".js": "text/javascript",
        ".wasm": "application/wasm",
        ".html": "text/html; charset=utf-8",
    }

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=directory, **kwargs)


print("Deneme alanı: http://127.0.0.1:%d/" % port)
http.server.ThreadingHTTPServer(("127.0.0.1", port), Handler).serve_forever()
