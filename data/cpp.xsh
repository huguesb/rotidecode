<?xml version="1.0" encoding="UTF-8"?>
<xsh lang="cpp" extensions="c;cc;cxx;cpp;h;hxx;hpp">
    <!--
        Behold the power of contexts

        A context can either be :
            * detached, as is "comment" below
            * anchored, as are "comment-multi" and "comment-single" below

        The difference is the presence of <enter> and <leave> tags.

        In practice, detached contexts are used to group any number of contexts,
        lists and tokens into a named group that can later be reused in multiple
        places via the <embed> tag.

        Anchored contexts are usually unfit for direct embedding (it remains do-
        able but will result in mismatching <enter> and <leave> tags) and behave
        very much like contexts from QCE2
    -->
    <context name="comment">
        <context name="comment-multi" format="comment">
            <enter paren="comment:open" fold="1">/\*</enter>
            <leave paren="comment:close" fold="1">\*/</leave>
        </context>

        <context name="comment-single" format="comment" stayOnLine="true" >
            <enter>//</enter>
            <leave noConsume="true">\n</leave>

            <context name="evilJump" format="danger" >
                <enter>\\\n</enter>
                <leave noConsume="false">\n</leave>
                <token>\\\n</token>
            </context>
        </context>
    </context>

    <embed>cpp:comment</embed>

    <context name="preprocessor" format="preprocessor" stayOnLine="true" >
        <enter>#[ \t]*(if|ifdef|else|elif|endif|define|include|pragma|error|warning)</enter>
        <leave>\n</leave>
        <embed>cpp:comment</embed>
        <token format="escapeseq" >\\\n</token>
    </context>

    <context name="string" format="text">
        <enter>&quot;</enter>
        <leave>&quot;</leave>
        <token format="escapeseq" noWordBoundaryCheck="true">\\[avtn&apos;&quot;\\]</token>
    </context>

    <!--
        Parentheses are a very important part of QCE generic syntax engine.
        Every token can be an instance of a parenthesis (class). This is done
        by giving an appropriate value to the "paren" attribute of the <token>
        element.

        $parenClassName$:$parenType$

        where :
            * $parenClassName$ is the name of the parenthesis (class)
            * $parenType$ is the parenthesis type of this token :
                - open
                - close
                - boundary  (both open and close)

        The attributes of a parenthesis (class) can be set through the attributes
        of any token that instantiates it. Although it is possible to give different
        values in different instanciations, in the end, all instances will share
        the same values for these attributes (the last value in the file always
        win). Therefore it is recommended to either only set the attributes once
        per parenthesis (class) or to provide the same values in all instances
        to avoid confusion.

        The attributes are :
            * match  : enable parenthesis matching [boolean, default: 1]
            * fold   : enable block folding        [boolean, default: 0]
            * indent : enable auto-indent          [boolean, default: 0]
            * weight : parenthesis weight          [pos int, default: 0]
    -->
    <token paren="curly:open" format="extra-keyword" fold="1" weight="2" >\{</token>
    <token paren="curly:close" format="extra-keyword" fold="1" weight="2" >\}</token>
    <token paren="round:open" format="extra-keyword" weight="1" >\(</token>
    <token paren="round:close" format="extra-keyword" weight="1" >\)</token>
    <token paren="square:open" format="extra-keyword" weight="1" >\[</token>
    <token paren="square:close" format="extra-keyword" weight="1" >\]</token>

    <list name="keywords/storage" format="keyword">
        <token>const</token>
        <token>volatile</token>
        <token>extern</token>
        <token>static</token>
        <token>mutable</token>
        <token>auto</token>
        <token>register</token>
        <token>inline</token>
    </list>

    <list name="keywords/data" format="keyword">
        <token>signed</token>
        <token>unsigned</token>
        <token>bool</token>
        <token>char</token>
        <token>int</token>
        <token>long</token>
        <token>float</token>
        <token>double</token>
        <token>enum</token>
        <token>union</token>
        <token>wchar_t</token>
        <token>void</token>
    </list>

    <list name="keywords/blocks" format="keyword">
        <token>for</token>
        <token>while</token>
        <token>do</token>
        <token>if</token>
        <token>else</token>
        <token>switch</token>
        <token paren="case:boundary" fold="1" indent="1" >case</token>
        <token paren="case:boundary" fold="1" indent="1" >default</token>
        <token>break</token>
        <token>continue</token>
        <token>try</token>
        <token>catch</token>
        <token>throw</token>
        <token>goto</token>
    </list>

    <list name="keywords/oop" format="keyword">
        <token>new</token>
        <token>delete</token>
        <token>class</token>
        <token>struct</token>
        <token>const_cast</token>
        <token>dynamic_cast</token>
        <token>static_cast</token>
        <token>reinterpret_cast</token>
        <token>operator</token>
        <token>virtual</token>
        <token>this</token>
        <token>explicit</token>
        <token>friend</token>
        <token>namespace</token>
        <token>using</token>
        <token>public</token>
        <token>private</token>
        <token>protected</token>
    </list>

    <list name="keywords/others" format="keyword">
        <token>asm</token>
        <token>false</token>
        <token>true</token>
        <token>template</token>
        <token>typename</token>
        <token>return</token>
        <token>sizeof</token>
        <token>typeid</token>
        <token>typedef</token>
    </list>

    <list name="keywords/Qt" format="extra-keyword">
        <token>foreach</token>
        <token>forever</token>
        <token>emit</token>
        <token>signals</token>
        <token>slots</token>
        <token>connect</token>
        <token>SIGNAL</token>
        <token>SLOT</token>
        <token>Q_OBJECT</token>
        <token>Q_ENUM</token>
        <token>Q_PROPERTY</token>
        <token>Q_SIGNALS</token>
        <token>Q_SLOTS</token>
    </list>

    <!--
        The purpose of list is to reduce noise/verbosity by :
            * applying the same format to a group of tokens
            * simplifying child regexps when prefix and/or suffix are involved
    -->
    <list name="ansi/trigraph" format="extra-keyword" prefix="\?\?">
        <token>\(</token>
        <token>\)</token>
        <token>&lt;</token>
        <token>&gt;</token>
        <token>=</token>
        <token>/</token>
        <token>&apos;</token>
        <token>!</token>
        <token>-</token>
    </list>

    <!--
        Tokens are the most important primitive of QCE3 generic syntax engine.
        They can match regular expressions with some restrictions :
            * NO numeric quantifiers (only regular quantifiers : ? * +)
            * NO lookahead
            * NO backreferences
            * NO explicit word boundary match (by default tokens only match
            when start and end are at word boundaries but this can be disabled
            via the noWordBoundaryCheck attribute)
        (The first limitation might be lifted in the future, the others are
        inherent to the DFA-based engine and trying to remove them would result
        in huge slowdown (up to several orders of magnitude...)).
    -->
    <token name="data/float" format="numbers">-?\d*\.\d+f?</token>
    <token name="data/hexadecimal" format="numbers">0[xX][0-9a-fA-F]+[ulUL]*</token>
    <token name="data/octal" format="numbers">0[0-7]+</token>
    <token name="data/decimal" format="numbers">-?(0|[1-9][0-9]*)[ulUL]*</token>
</xsh>
