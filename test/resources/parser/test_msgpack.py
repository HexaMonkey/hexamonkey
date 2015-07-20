import msgpack

with open("test_msgpack.msgpack", 'w') as f:
    msgpack.dump([b'spam', u'egg', True, False], f, use_bin_type=True)
