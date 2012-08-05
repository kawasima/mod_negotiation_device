mod_negotiation_device
======================

同じURLへのアクセスに対して、デバイスごとに異なるコンテンツを返すためのモジュールです。

デバイスの判定はUserAgentを用いて行います。

    NegotiationDevice [+-]device... Redirect path

のように対象としたい、または対象外にしたいデバイスを並べて書きます。最後にマッチしたときに、
リダイレクトするパスを書きます。

    <Directory /app>
        NegotiationDevice +ipone /iphone
        NegotiationDevice +android /android
    </Directory>


デバイスの条件は複数書くことができます。例えばスマートフォンのうちiPhone以外を対象とする場合は以下のように書きます。

    <Directory /app>
        NegotiationDevice +smartphone -iphone /not_iphone
    </Directory>
