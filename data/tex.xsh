<?xml version="1.0" encoding="UTF-8"?>
<xsh lang="tex" extensions="tex">
    <context name="text" format="normal">
        <token name="command" format="keyword">\\[a-zA-Z]+</token>
        <token name="parametrizedcommand" format="keyword">
            <subtoken>\\[a-zA-Z]+</subtoken>
            <subtoken format="preprocessor">(\[[^\[\]]*\])?</subtoken>
            <subtoken format="extra-keyword">{</subtoken>
            <subtoken format="text">[^{}]*</subtoken>
            <subtoken format="extra-keyword">}</subtoken>
        </token>
        <context name="math" format="numbers">
            <enter format="extra-keyword" paren="math:open" fold="1" indent="1">
                <subtoken>\\begin{</subtoken>
                <subtoken format="comment">displaymath</subtoken>
                <subtoken>}</subtoken>
            </enter>
            <leave format="extra-keyword" paren="math:close" fold="1" indent="1">
                <subtoken>\\end{</subtoken>
                <subtoken format="comment">displaymath</subtoken>
                <subtoken>}</subtoken></leave>
            <embed>tex:math</embed>
        </context>
        <context name="math" format="numbers">
            <enter format="preprocessor" paren="math2:open" fold="1">$</enter>
            <leave format="preprocessor" paren="math2:close" fold="1">$</leave>
            <embed>tex:math</embed>
        </context>
    </context>
    <context name="math" format="numbers">
        <context name="text" format="normal" >
            <enter format="keyword" paren="text:open" fold="1">\\text{</enter>
            <leave format="keyword" paren="text:close" fold="1">}</leave>
            <embed>tex:text</embed>
        </context>
    </context>
    <embed>tex:text</embed>
</xsh>