import msgpack, math



with open("test_msgpack.msgpack", 'w') as f:
    def write(str):
        f.write(str)
    
    def dump(item, **kwargs):
        msgpack.dump(item, f, **kwargs)
        
    def dumpList(list):
        for item in list:
            dump(item)

    write(b"\xdc\x00\x24");
    
    dumpList([None,
              False,
              True,
              42,
              -5,
              150,
              33000,
              2000000])
    write(b"\xcf\x00\x07\x1a\xfd\x49\x8d\x00\x00") # issue with msgpack module with large integers
    dumpList([-60,
              -3000,
              -200000,
              -20000000000L])

    dump(math.pi, use_single_float = True);
    dump(math.pi, use_single_float = False);

    dump("Hello world!");

    string = "Hello world!"; 
    write(b"\xd9\x0c");
    write(string);
    write(b"\xda\x00\x0c");
    write(string);
    write(b"\xdb\x00\x00\x00\x0c");
    write(string);

    string = "\x00"*8
    write(b"\xc4\x08");
    write(string);
    write(b"\xc5\x00\x08");
    write(string);
    write(b"\xc6\x00\x00\x00\x08");
    write(string);

    list = [1,2,3,4,5,6,7,None];
    dump(list);

    write(b"\xdc\x00\x08");
    dumpList(list);

    write(b"\xdd\x00\x00\x00\x08");
    dumpList(list);

    write(b"\x84");
    dumpList(list);

    write(b"\xde\x00\x04");
    dumpList(list);
    
    write(b"\xdf\x00\x00\x00\x04");
    dumpList(list);

    write(b"\xd4\x2a");
    write("\x00");

    write(b"\xd5\x2a");
    write("\x00"*2);

    write(b"\xd6\x2a");
    write("\x00"*4);

    write(b"\xd7\x2a");
    write("\x00"*8);

    write(b"\xd8\x2a");
    write("\x00"*16);

    string = "\x00"*8
    write(b"\xc7\x08\x2a");
    write(string);
    write(b"\xc8\x00\x08\x2a");
    write(string);
    write(b"\xc9\x00\x00\x00\x08\x2a");
    write(string);
