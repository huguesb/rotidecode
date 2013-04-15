<?xml version="1.0" encoding="UTF-8"?>
<xsh lang="doxygen" extensions="dox">
    <context name="keywords">
        <list format="doxygen/keyword" prefix="[@\\]">
            <token>a</token>
            <token>dot</token>
            <token>addindex</token>
            <token>dotfile</token>
            <token>addtogroup</token>
            <token>e</token>
            <token>anchor</token>
            <token>else</token>
            <token>arg</token>
            <token>elseif</token>
            <token>attention</token>
            <token>em</token>
            <token>author</token>
            <token>endcode</token>
            <token>b</token>
            <token>endcond</token>
            <token>brief</token>
            <token>enddot</token>
            <token>bug</token>
            <token>endhtmlonly</token>
            <token>c</token>
            <token>endif</token>
            <token>callgraph</token>
            <token>endlatexonly</token>
            <token>category</token>
            <token>endlink</token>
            <token>class</token>
            <token>endmanonly</token>
            <token>code</token>
            <token>endverbatim</token>
            <token>cond</token>
            <token>endxmlonly</token>
            <token>copydoc</token>
            <token>enum</token>
            <token>date</token>
            <token>example</token>
            <token>def</token>
            <token>exception</token>
            <token>defgroup</token>
            <token>deprecated</token>
            <token>dir</token>
            <token>dontinclude</token>
            <token>file</token>
            <token>fn</token>
            <token>protocol</token>
            <token>hideinitializer</token>
            <token>htmlinclude</token>
            <token>htmlonly</token>
            <token>ref</token>
            <token>if</token>
            <token>relates</token>
            <token>ifnot</token>
            <token>relatesalso</token>
            <token>image</token>
            <token>remarks</token>
            <token>include</token>
            <token>return</token>
            <token>includelineno</token>
            <token>retval</token>
            <token>ingroup</token>
            <token>sa</token>
            <token>internal</token>
            <token>section</token>
            <token>invariant</token>
            <token>see</token>
            <token>interface</token>
            <token>showinitializer</token>
            <token>test</token>
            <token>namespace</token>
            <token>throw</token>
            <token>nosubgrouping</token>
            <token>todo</token>
            <token>note</token>
            <token>typedef</token>
            <token>overload</token>
            <token>union</token>
            <token>p</token>
            <token>until</token>
            <token>package</token>
            <token>var</token>
            <token>page</token>
            <token>verbatim</token>
            <token>par</token>
            <token>verbinclude</token>
            <token>paragraph</token>
            <token>version</token>
            <token>param</token>
            <token>warning</token>
            <token>post</token>
            <token>weakgroup</token>
            <token>pre</token>
            <token>xmlonly</token>
            <token>xrefitem</token>
            <token>property</token>
            <token>mainpage</token>
            
            <token>&amp;</token>
            <token>@</token>
            <token>\$</token>
            <token>\\</token>
            <token>%</token>
            <token>#</token>
            <token>~</token>
            <token>&lt;</token>
            <token>&gt;</token>
            <token>{</token>
            <token>}</token>
            <token>f\$</token>
            <token>f\[</token>
            <token>f\]</token>
        </list>
    </context>
    
    <context name="comment">
        <context name="multi" format="doxygen">
          <enter paren="comment:open" fold="1">/\*[*!][^*]</enter>
          <leave paren="comment:close" fold="1">\*/</leave>
          <embed>doxygen:keywords</embed>
          <embed>xml</embed>
        </context>
        
        <context name="single" format="doxygen">
          <enter>//[/!]</enter>
          <leave>\n</leave>
          <embed>doxygen:keywords</embed>
          <embed>xml</embed>
        </context>
    </context>
    
    <embed>comment</embed>
</xsh>
