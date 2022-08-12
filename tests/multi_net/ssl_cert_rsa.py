# Simple test creating an SSL connection and transferring some data
# This test won't run under CPython because CPython doesn't have key/cert

try:
    import ubinascii as binascii, usocket as socket, ussl as ssl
except ImportError:
    print("SKIP")
    raise SystemExit

PORT = 8000


# This self-signed key/cert pair is randomly generated and to be used for
# testing/demonstration only.  You should always generate your own key/cert.

# To generate a new self-signed key/cert pair with openssl do:
# $ openssl req -x509 -newkey rsa:4096 -keyout rsa_key.pem -out rsa_cert.pem -days 365 -node
#
# Convert them to DER format:
# $ openssl rsa -in rsa_key.pem -out rsa_key.der -outform DER
# $ openssl x509 -in rsa_cert.pem -out rsa_key.der -outform DER
#
# Then convert to hex format, eg using binascii.hexlify(data).

cert = binascii.unhexlify(
    b"308205d7308203bfa003020102020900bc63b48a700c3d49300d06092a864886f70d01010b050030"
    b"8181310b3009060355040613024155310c300a06035504080c03466f6f310c300a06035504070c03"
    b"42617231143012060355040a0c0b4d6963726f507974686f6e310c300a060355040b0c03666f6f31"
    b"16301406035504030c0d657370686f6d652e6c6f63616c311a301806092a864886f70d010901160b"
    b"666f6f406261722e636f6d301e170d3232303731323138303031335a170d32333037313231383030"
    b"31335a308181310b3009060355040613024155310c300a06035504080c03466f6f310c300a060355"
    b"04070c0342617231143012060355040a0c0b4d6963726f507974686f6e310c300a060355040b0c03"
    b"666f6f3116301406035504030c0d657370686f6d652e6c6f63616c311a301806092a864886f70d01"
    b"0901160b666f6f406261722e636f6d30820222300d06092a864886f70d01010105000382020f0030"
    b"82020a0282020100ce3c0f730ab34432ce605ab44d4ac0aafd8a6243133eab0dcc9d444ab7d9ff66"
    b"a6815a101d2d3cbd72140afc34f8c3caedce16e9528350f3e0e56343f248507d82e41b51abb515cb"
    b"f60e5a619f2dbca8684d174c3b0951e2c7ba576c7fb06453a3597755810a6a4c45eb0925c855ab53"
    b"7785df46bf29145871330ff0641a101a24f0830c20bae865ba8bb32606caac4555812acf19f59553"
    b"349ce70fb7ff63512f0444f8f41b973183eabf9679903087c6cd69dc3adcbe754dd0207ea57c50e9"
    b"2d800bce6258d1618bb749d3fc01239b6d1af6d3f9cada3acbb312a1d85a59cfabd28b2e572c56a4"
    b"818ce170ca2b781a04749c6239206c64ad9e057484143a4c52bdef6189c46405c1a9642489cb640a"
    b"937adfc2687578dfa2b40ebafa05213642a1ccbc265557cd40de53324cff1bfba6f5c215f657b8f9"
    b"f2260ab6293625d0e203bba975bc7ac6dff3e604c9b0d2a2a4ba5941c0dc8d2e0e9439c56447b404"
    b"8c0e6cfb03517742ff6f7c2140a05954aa1e29247d1ae8bfd7db0db8dd45d095710fb78284ede285"
    b"0fc0c21235406af83e6044addf9385316403e2a25442b9ffbfc7b01c6c9292e5a3531e6a48496c01"
    b"6de1373334a52f01b7c6a0ece1261936788d2161c53a8985a0946d6d319225b230d96d055ea4692f"
    b"eb71fdaf4b775ac9fbc38e1b943e6617cf61d33e930ab288a3ea4730b4f2784a8018e0dfc8a11e73"
    b"0203010001a350304e301d0603551d0e04160414bc6048fe3cd278257e8b7c90dedbbce8369b20b8"
    b"301f0603551d23041830168014bc6048fe3cd278257e8b7c90dedbbce8369b20b8300c0603551d13"
    b"040530030101ff300d06092a864886f70d01010b0500038202010009238354b43379a3d2b56e928c"
    b"ac8ea28e2c01cf8148e54c0bbd4055e2e57d578697d1e2c392f1fe3bc9211d4f27ed1be631e7547a"
    b"6390d7f121a9e20a195fdda73f755188b16cf39714924a9686dd7cc749421335038c0640c2c6b15d"
    b"f44d74d94a97285ee2a7b075ccc9d9d632e2a5906030cf59bde14ab10660b7cf47ec9d7ae2f35963"
    b"454f76735a3dac12a4a4c907183e9ccf3e07d59484c182e67edc7c35ce15c7e1072fae8c9965a126"
    b"1a1f31147d4af8d1ebf8ee7c142badfe67e31fb324a79a29bc94e89370b70d8cf7cd2b2aa427a49f"
    b"77849891e7c4d5911f6fda52733a3c169b0188c2d9918f296dd8e234f8962f0db5e47c6159448045"
    b"4e2d9a5850d4c696a0fb3b66534a4591c49dda8cc6f1b0008c625aa5e0091ecfbd51d9715c60b85e"
    b"4e89d4a6cfabb2acdf81518eb61403b8f8767c5c00216f730e08f22959dff695a081cc726c4ab35a"
    b"e3f6538a231f831a6e91206f3b691a94bdf95343ec02ef7aac42da2a70846cd5f13dd2955a5f1737"
    b"a4c3c6c03b041d334c1dadd1e305f07c83b4b4e0509ec1d23e95f820290942eaaf8bea304cd5a505"
    b"8fc0d4624ff1ffe1348e7bc54c756a12acb258eb5e7426fb062a82b88ec274c9c13b3eff8b010947"
    b"62e166f490cd25b14e762db708785859a337d8fd0008fe602a90e2933cded3359e98ce3fbc041208"
    b"66bd4d96d6b6f7f53def854d40021196b7a06b"
)

key = binascii.unhexlify(
    b"308209290201000282020100ce3c0f730ab34432ce605ab44d4ac0aafd8a6243133eab0dcc9d444a"
    b"b7d9ff66a6815a101d2d3cbd72140afc34f8c3caedce16e9528350f3e0e56343f248507d82e41b51"
    b"abb515cbf60e5a619f2dbca8684d174c3b0951e2c7ba576c7fb06453a3597755810a6a4c45eb0925"
    b"c855ab537785df46bf29145871330ff0641a101a24f0830c20bae865ba8bb32606caac4555812acf"
    b"19f59553349ce70fb7ff63512f0444f8f41b973183eabf9679903087c6cd69dc3adcbe754dd0207e"
    b"a57c50e92d800bce6258d1618bb749d3fc01239b6d1af6d3f9cada3acbb312a1d85a59cfabd28b2e"
    b"572c56a4818ce170ca2b781a04749c6239206c64ad9e057484143a4c52bdef6189c46405c1a96424"
    b"89cb640a937adfc2687578dfa2b40ebafa05213642a1ccbc265557cd40de53324cff1bfba6f5c215"
    b"f657b8f9f2260ab6293625d0e203bba975bc7ac6dff3e604c9b0d2a2a4ba5941c0dc8d2e0e9439c5"
    b"6447b4048c0e6cfb03517742ff6f7c2140a05954aa1e29247d1ae8bfd7db0db8dd45d095710fb782"
    b"84ede2850fc0c21235406af83e6044addf9385316403e2a25442b9ffbfc7b01c6c9292e5a3531e6a"
    b"48496c016de1373334a52f01b7c6a0ece1261936788d2161c53a8985a0946d6d319225b230d96d05"
    b"5ea4692feb71fdaf4b775ac9fbc38e1b943e6617cf61d33e930ab288a3ea4730b4f2784a8018e0df"
    b"c8a11e73020301000102820201008efa0e8fe81c2e2cb6ed10152dfca4242750581d3e6b54f56524"
    b"a6a2d2613cf2727efcec6cfddebd4c285f1148bc2a2936c28919cb0da502dea8c92fe2f9856bee61"
    b"ac1aebdac838b5e66f7c7c799df07716f30ef362dbb5485884a180c8ce5539cb1db35699dce5f217"
    b"27295d811f1ce7a115111c1823b5c90ce880f5352872a7a76282f6f1fd8a015136ab274c3d30783d"
    b"eb6ad7096e33d826eafdf7c70398d5eab4d28f91cd3913c69c7a7ade9ef692b9f8292959be64dec4"
    b"6ab2c291b41a6464004b5ddd4b93bfe41b37eedeef4ba2d16dcbb9c28b96f57fb96c20ed4a9471ff"
    b"ae643b254f100f8c9702b5f67af6369e8d887f285e5d520c5aa5d3a79e5de96432e6d2e3dea68e58"
    b"208c075fb119c6d3d4149b7e1247208d6b337c70272befc41d57f278618f1a82de337173346dc135"
    b"4d80a7c9075af99dbb2a14733c06b71600c6677a6bb28c0e4fc63db622228047a2cb7474dc8141c3"
    b"5f3a597c3e2bca9911d28eb9fd1a0c915e9f9c1cfd643d4fd8cac867f215380168ec37b8cfa28564"
    b"e6288ab04a7d67ca44b4c8375214a7ffaa1e6be92c4b138fcfd6beaba251b31a50a6e2ef241c9554"
    b"a1dc710b4acb63e749f5849e53d3f4915c6eb2a9a009bab04e932841ab34ae29eb000a08777d6399"
    b"169c2dc3d7952df5bc2d06e90a32139c6a2793d3817e4feadac2ccac554d383a8d41569140c29168"
    b"89220d3a5e410282010100fd603d18feef7aac61bda3b674a57ab38748bcde5c3efdba2279638f8e"
    b"a413cc26b9dda0375c116a8798a295b2c283aaaad7cca0dbd9bb3322a9a815f6d0aa5fc4f9aff8fb"
    b"da8ff914091ede7aefdb07a119c9b2e2b2bda776ac497060b8e88a82eb20c62f26f343566697726e"
    b"71aa46fd4efad6f42fc8a478856324d72cbf5eb3918317162d6fc2cfd775969a2077759fa2c8220d"
    b"acdc2ebb03ec39feed3f2b415449cbf40a7126bcf01d1068e3a45ec01181f2c68d7e05b4720bfe4a"
    b"308e1648123c91214a5f8dfce58727c4cd9396a8b403b733a717449b2f1970db97a3b8467271ffa6"
    b"e8c7cc9e2e1c0f789284ae9efe77eaac01131463c9c1329a1ba3530282010100d05ed6ab9b9fbdf7"
    b"a0f5f91f68dc3bac5789332d6ece46103fb1ef109fc972fdc99edf3107a23d66d1cdfe6bdddfd1bb"
    b"3952ccd10b5c20ad1b3e0aa6a51271ecf3a7ef2a65e029f5d77f238d1235b52a9dca3451c165d70a"
    b"99cbaea5c610e5455979696db769191e7cf2db21f641959e4ba1c5c0aae260c724962b6ac2621d92"
    b"e9df7adeb82b522d37b42cb454003bbe60d9915bf7737aeccf88c7ed1263a22f431a734e61fe7173"
    b"a937ddf76ad2a79994c05238defc15f6846858e9edf27ae2a567c7c5c735ea5d2fbef65a2195bc05"
    b"d82cbf06a477b29c84c92e8054c2bb25d8c6f19d43ef5fd1fce13c2cdbc361c39baec37b399200b3"
    b"2d4a6798ba0b546102820101008e41492c4e7daff7368d1d6c64034067a94dca5461a0301e201add"
    b"2e0d5ccb8cb435685bfa98e362572cf8236a10d191b187a568aee688b6c60050d1bc181d7fd57c86"
    b"33195bf5b7576b637c6fb358dae8b52ccc15815affb99e334137dcb91a833475db2f4004164b5d20"
    b"2c6c1bbf094a50dc7e70ec9f0ed067bb6944b1e7e3c897aaecfc53984add1c4ff5b525034cf3ca95"
    b"e8a09aeba804f1c7e02be391b2bc641166c3e654eef5e72dba37d98f406f3fa520e41f2ea10f5574"
    b"ac5984f75145378fefbfac1d07fff3f234fec698d55e746b1da18f6f7de24ec84ed7cb446d428820"
    b"bef33c00693e6a0ef114b5d66e9fefa8ee059238df1ac37c87e7841ae7028201000721a7d139c34e"
    b"da21cd295894db2cc3aa3f4cdc1a35bf1a2143f2bdabea56202f7d5b802f15b36a4875f76633b2cc"
    b"57cf0f71691a2d6e04deb0d1e68031d06a5eb079b406c6944910b60e3e6ec81dca369a4c0e1c4363"
    b"07bed9c4c171b4f453da4b187ba3d25a04bc1c07b9f2d6adcb3c256e4238d7049eec36a387c4dd5c"
    b"cbc16b5fa62dc175cf8c5f83442cb7d153a3b6ee8daa3b6e929a4bc123f1042df1d6271a992d2b6b"
    b"309d33074ac7822c304a72069e61ab590915e10862013dd24cdd825ec8fb17724cfc2c59fc1db825"
    b"3641fece0ee9241b9dd5c198f0d575d0b7ebe26b3489b5b09edc3bcd366fd3110e83ce886c383d31"
    b"feefe6e302cc2345210282010008f77a33d0081e9be3c1b1ac8b8e0eebb72df2eb69b95d2ed74935"
    b"b9dab8e17023cc38465354023c5183b51a6a20288fbb2181172be1c2fdb8b444419454e5b37f7f3b"
    b"df11e28cf4746b25534eb62f7e87bbbf28eda37024368b3897fbc661b40a93e04a183db9219c04a8"
    b"7643edf5d8b5dbfe3d424e91d558d5e3e2fa02ce1984ee69fb8518470eee2e7db0e1df5ac4571f78"
    b"a7a2529bc1fef5e32d46994869a8d8cc47869e174d84e7976be8ebb88f2ccb71a603a8bdb06af3eb"
    b"2ddbd62082f40d7987e47f2e321eb5eb2a28fefab263409f89dc97ebc723a1b751418cdd3ea684ba"
    b"8b17a330a306a6fbcf51ba83563aed85a4f886fff1a22423748d83798c"
)

# Server
def instance0():
    multitest.globals(IP=multitest.get_network_ip())
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(socket.getaddrinfo("0.0.0.0", PORT)[0][-1])
    s.listen(1)
    multitest.next()
    s2, _ = s.accept()
    s2 = ssl.wrap_socket(s2, server_side=True, key=key, cert=cert)
    print(s2.read(16))
    s2.write(b"server to client")
    s2.close()
    s.close()


# Client
def instance1():
    multitest.next()
    s = socket.socket()
    s.connect(socket.getaddrinfo(IP, PORT)[0][-1])
    s = ssl.wrap_socket(s, cert_reqs=ssl.CERT_REQUIRED, cadata=cert)
    s.write(b"client to server")
    print(s.read(16))
    s.close()
