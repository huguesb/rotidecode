<?xml version="1.0" encoding="UTF-8"?>
<xsh lang="xml" extensions="xml;xsh">
    <context name="comment" format="comment" >
        <enter paren="xmlcomment:open" indent="1" fold="1">&lt;!--</enter>
        <leave paren="xmlcomment:close" indent="1" fold="1">--&gt;</leave>
    </context>

    <context name="cdata" format="comment" >
        <enter paren="xmlcdata:open" indent="1" fold="1">&lt;!\[CDATA\[</enter>
        <leave paren="xmlcdata:close" indent="1" fold="1">\]\]&gt;</leave>
    </context>

    <context id="preprocessor" format="preprocessor" >
        <enter paren="xmlpreproc:open" indent="1" fold="1">&lt;\?xml</enter>
        <leave paren="xmlpreproc:close" indent="1" fold="1">\?&gt;</leave>
    </context>

    <context id="doctype" format="text" >
        <enter>&lt;!DOCTYPE</enter>
        <leave>&gt;</leave>
    </context>

    <token id="entity" format="numbers">&amp;[a-zA-Z0-9\-_:]+;</token>

    <context id="closetag" format="numbers">
        <enter format="keyword" >&lt;/[a-zA-Z0-9\-_:]+</enter>
        <leave format="keyword" paren="xmltag:close" indent="1" fold="1" >&gt;</leave>
    </context>

    <context id="opentag" format="numbers">
        <enter format="keyword" >&lt;[a-zA-Z0-9\-_:]+</enter>
        <leave format="keyword" paren="xmltag:open" indent="1" fold="1" >&gt;</leave>
        <leave format="keyword" >/[ \t]*&gt;</leave>

        <token format="preprocessor">[a-zA-Z0-9\-_:]+[ \t]*=</token>

        <context id="data/string1" format="text" >
            <enter>&quot;</enter>
            <leave>&quot;</leave>
        </context>

        <context id="data/string2" format="text" >
            <enter>&apos;</enter>
            <leave>&apos;</leave>
        </context>
    </context>
</xsh>