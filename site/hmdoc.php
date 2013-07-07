<?php require_once("head.php"); ?>
<?php require_once("navigation.php"); ?>
<?php require_once("header.php"); ?>
			<h2>HMscript Documentation</h2>
			<h3 id="WhatIs">What is HMscript?</h3>
			<p>HMscript is an imperative object-oriented programming language used to describe how files of a given container format should be parsed. It can be interpreted by HexaMonkey in order to parse files.</p>
			<p>The basic principle is that containers format files are composed of nested boxes. A model object is therefore used to describe the parsing, the parsers are written as enhanced class definitions which make them very simple and expressive while still offering high-level functionnalities such as loops, conditional branching and local variables.</p>
			
			<h3 id="GettingStarted">Getting started</h3>
			<p>First of all create a blank file with a .hm extension and put in the <em>script</em> subfolder of your HexaMonkey installation folder. Then basic layout of the file shall be:</p>
			<ol>
				<li>Format detection directives</li>
				<li>Module importations</li>
				<li>Class definitions</li>
			</ol>
			<p>For instance a very basic HMscript file would be:</p>
<pre><span class="comment">//(1) Format detection directives</span>
<span class="keyword">addExtension</span> dummy

<span class="comment">//(2)Module importation</span>
<span class="keyword">import</span> bestd

<span class="comment">//(3) Class definitions</span>
<span class="keyword">class</span> <span class="type">DummyFile</span> <span class="keyword">as</span> <span class="type">File</span>
{
    <span class="type">uint</span>(<span class="number">32</span>) <span class="variable">payload</span>[];
}</pre>
			<p>This description is interpreted as so:</p>
			<ol>
				<li>This tells to HexaMonkey that if the file has <strong>dummy</strong> as an extension then this module should be used. (<a href="#FormatDetection">More on format detection</a>)</li>
				<li>This loads the <strong>bestd</strong> module which stands for <strong>big endian standard</strong> (<a href="http://en.wikipedia.org/wiki/Endianness">http://en.wikipedia.org/wiki/Endianness</a>). It allows the use of <strong>uint</strong>. (<a href="#Modules">More on modules</a>)</li>
				<li>This declares that the file should be parsed an array of 32 bits unsigned integers. (<a href="#ClassDefinition">More on classes</a>)</li>
			</ol>
			
			<h3 id="FormatDetection">Format detection directives</h3>
			<p>The format detection directives are standardized predicates applied to files to check if the module should be loaded to parse them. The standardizes operation are the following:</p>
			<ul>
				<li>
				<strong>addMagicNumber</strong> <em>magicNumber</em> : This operation relies on magic numbers a.k.a. file signatures (<a href="http://en.wikipedia.org/wiki/Magic_number_%28programming%29">http://en.wikipedia.org/wiki/Magic_number_(programming)</a>). The <em>magicNumber</em> should be a succession of bytes that must be present in the beginning of the file. The bytes should be written in hex code and space separated. If a value of a byte can vary you can mark it as xx. If a file matches several magic numbers the longest will be chosen. You can find <a href="http://www.garykessler.net/library/file_sigs.html">here</a> a large number of magic numbers.
				<p>Example :</p>
				<pre><span class="keyword">addMagicNumber</span> 00 00 00 xx 66 74 79 70 <span class="comment">//Magic number for mp4: ....ftyp</span></pre>
				</li>
			
				<li>
				<strong>addSyncbyte</strong> <em>syncbyte</em> <em>periodicity</em> : This operation searches for a byte repeating with a fixed periodicity. This is useful for formats composed of packets beginning with a fixed synchronisation byte.
				<p>Example :</p>
<pre><span class="keyword">addSyncbyte</span> 0x47 188 <span class="comment">//Syncbyte for mpeg2-ts : packets of size 188 bytes 
                     //beginning by the syncbyte 0x47</span></pre>
				</li>
			
				<li>
				<strong>addExtension</strong> <em>extension</em> : This operation associate files with a given extension with this module.
				<p>Example :</p>
				<pre><span class="keyword">addExtension</span> mp4 <span class="comment">//Association of the files with a .mp4 extension</span></pre>
				</li>
			</ul>
			
			<p>Several directives can be specified for a same file. Globally the first positive result will stand, the priority being magicNumber first, then syncbyte and finally extension.</p> 
			
			<h3 id="Modules">Modules</h3>
			
			<p>Modules can be imported very simply by using the command : <strong>import</strong> <em>moduleName</em>. All the classes available in the imported module will be directly available. If there is a naming conflict, the last class definition will be chosen.</p>
			
			<h4>Default Module</h4>
			
			<p>This module is always loaded, it contains fundamental classes :</p>
			<ul>
				<li><strong>File</strong> : This class will be called to parse the file. It should be specified to define the top-level structure of the format. (<a href="#Inheritance">more on inheritance</a>).</li>
				
				<li><strong>Array</strong>(<em>type</em>, <em>size</em>) : Objects of type <em>type</em> are parsed in the area of size <em>size</em> given (in bits). <em>size</em> is optionnal and defaults to the remaining size available in the container object. For convenience <pre  class="inline"><span class="type">Array</span>(<span class="type">type</span>) <span class="variable">array</span>;</pre> is strictly equivalent to <pre  class="inline"><span class="type">type</span> <span class="variable">array</span>[];</pre></li>
				
				<li><strong>Tuple</strong>(<em>type</em>, <em>count</em>) : Objects of type <em>type</em> are parsed <em>count</em> times. For convenience <code><span class="type">Tuple</span>(<span class="type">type</span>, <span class="number">8</span>) <span class="variable">tuple</span>;</code> is strictly equivalent to <code><span class="type">type</span> <span class="variable">tuple</span>[<span class="number">8</span>];</code></li>
				
				<li><strong>Data</strong>(<em>size</em>) :  Uninterpreted data is parsed in the area of size <em>size</em> given (in bits). <em>size</em> is optionnal and defaults to the remaining size available in the container object.</li>
			</ul>
			
			<h4>Standard Module</h4>
			
			<p>This module contain basic classes such as integers, strings, floats... Two versions of this module exists : a big-endian one under the code name <strong>bestd</strong> and a little-endian one under the code name <strong>lestd</strong> (<a href="http://en.wikipedia.org/wiki/Endianness">http://en.wikipedia.org/wiki/Endianness</a>).
			<ul>
				<li><strong>int</strong>(<em>size</em>, <em>base</em>), <strong>uint</strong>(<em>size</em>, <em>base</em>): An integer of size <em>size</em> given (in bits) is parsed. The <em>size</em> must be between 1 and 64. The value will be displayed with the <em>base</em> given, the bases can be 8, 10 and 16 and the default is 10.</li>
				<li><strong>float</strong>, <strong>double</strong>: A floating number of size 32 bits and 64 bits respectively is parsed. The parsing complies with the IEEE Standard for Floating-Point Arithmetic (IEEE 754).</li>
				<li><strong>String</strong>(<em>charCount</em>) : A string of characters is parsed. By default the parsing will stop the null character is reached or the end of the container. A fixed number of character can be set with the <em>charCount</em> parameter.</li>
				<li><strong>Bitset</strong>(<em>bitCount</em>) : A string of <em>bitCount</em> bits is parsed. The <em>size</em> must be between 1 and 64. The object can be manipulated as an unsigned integer.</li>
			</ul>
	
			<h3>Variable values</h3>
			<p>HMscript is weakly typed, uinitialized variables default to <code><span class="keyword">NULL</span></code>. Internally variable can either be of type :</p>
			<ul>
				<li><strong>unsigned integer</strong> coded on 64 bits which allows values from 0 to 18,446,744,073,709,551,615. It can be given in base 2 (<code><span class="number">0b10000</span></code>), base 8 (<code><span class="number">020</span></code>), base 10 (<code><span class="number">16</span></code>) or base 16 (<code><span class="number">0x10</span></code>).
				</li>
				<li><strong>signed integer</strong> coded on 64 bits with two's-complement which allows values from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807. It can be defined in the same way as unsigned integers but also allows negative integers : <code><span class="number">-0b10000</span></code>, <code><span class="number">-020</span></code>, <code><span class="number">-16</span></code> or <code><span class="number">-0x10</span></code>. Positive integer are stored as unsigned integers but are converted to signed integers if they become negative.
				</li>
				
				<li><strong>floating</strong> with double precision according to the IEEE Standard for Floating-Point Arithmetic (IEEE 754), ie 1-bit for sign, an exponent coded on 11 bits and a mantissa on 52-bits. It can be defined as so : <code><span class="number">1.2</span></code>, <code><span class="number">1.</span></code>, <code><span class="number">.1</span></code>, <code><span class="number">-1.</span></code>, <code><span class="number">2e-7</span></code>...</li>
				
				<li><strong>string</strong> coded as a null-terminated string of ASCII characters. It can be defined as so : <code><span class="string">"example"</span></code>.
				
				<li><strong>object type</strong> defined by a class with its parameters specified. In can be defined as so : <span class="type">className</span>(<em>value_1</em>, <em>value_2</em>...<em>value_k</em>). Be careful, even if k is 0 the <u>parenthesis are mandatory</u> in this case.</li>
				
				<li><strong>unknown</strong> which is limited to the reserved value <code><span class="keyword">NULL</span></code>. It represent the default value on initialisation and result to an operation with undefined behavior.</li>
			</ul>
			
			<p>Most operators are implemented and will have the same effect as in the c langage.</p>
			
			<h3 id="ClassDefinition">Class Definition</h3>
			<p>Classes are defined by a unique identifier and a number of parameters as so:</p>
<pre><span class="keyword">class</span> <span class="type">className</span>(<em>param_1</em>, <em>param_2</em> ... <em>param_n</em>) 
{
    <em>statements</em>
}
</pre>
			<p>If n is zero, the parenthesis are optional.</p>
			<p>The body of the class definition is a series of statements nested in curly braces which will be interpreted sequentially. The braces are optional if there is one or less statement. The different possible statements are:
			<ul>
				<li><p><strong>Declaration</strong> parses an object with a given object type:</p>
					<pre><em><span class="type">className</span></em>(<em>value_1</em>, <em>value_2</em>...<em>value_k</em>) <em><span class="variable">name</span></em>;</pre>
					<p>Here k can be strictly lower than n, in which case the remaining parameters are simply set to <span class="keyword">NULL</span>. If k is 0 the parenthesis are optional. Displaying a <em><span class="variable">name</span></em> is optional,  <span class="variable">*</span> can be used instead to create an anonymous object.</p>
					<p>The object type can also be defined through a variable. In which case the variable as to be surronded by parenthesis as so:
<pre><span class="keyword">var</span> i = <em><span class="type">className</span></em>(<em>value_1</em>, <em>value_2</em>...<em>value_k</em>);
(i) <em><span class="variable">name</span></em>;</pre>
				</li>
				<li><p><strong>Local variable declaration</strong> declares a variable and initialize is as so:</p>
				<pre><span class="keyword">var</span> i = <em>value</em>;</pre>
				<p>It is also possible to leave the variable uinitialized, in which case the value will be <code><span class="keyword">NULL</span></code>. The scope of the local variable is the rest of the class definition.</p>
				</li>
				<li><p><strong>Expression</strong> simply evaluates an expression. For instance :</p>
				<pre>i = <span class="number">12</span>;</pre>
				<pre>++i;</pre>
				</li>
				<li><p><strong>Conditional branchement</strong> executes one block or another depending on the value of a condition. It can be defined as such:
<pre><span class="keyword">if</span>(<em>condition</em>)
{
    <em>then_statements</em>
}
<span class="keyword">else</span>
{
    <em>else_statements</em>
}
</span></pre>
				<p>The <code><span class="keyword">else</span></code> is optional and so are the curly braces if there is only one statement. The condition is simply an expression that is implicitly converted to a boolean.</p>
				</li>
				<li><p><strong>Loop</strong> execute a block of statements while the condition is true . It can be defined as such:
<pre><span class="keyword">while</span>(<em>condition</em>)
{
   <em>statements</em>
}
</pre>
				<p>The <span class="keyword">for</span> notation can also be used:
<table>
<tr>
<td>
<pre><span class="keyword">for</span>(<em>initialisation_statement</em>; <em>condition</em>; <em>end_of_loop_statement</em>)
{
   <em>statements</em>
}
</pre>
</td>
<td>		
				<p>&equiv;</p>
</td>
<td>
<pre>
<em>initialisation_statement</em>;
<span class="keyword">while</span>(<em>condition</em>)
{
    <em>statements</em>
    <em>end_of_loop_statement</em>;
}
</pre>
</td>
</tr>
</table>					
				</li>
			</ul>

<h3>Object manipaluation</h3>
<h4>Member access</h4>
<p>Once members have been declared they can be accessed in different ways:</p>
<ul>
	<li><strong>By name</strong> : a member can be accessed by its name using directly its identifier as such <code><span class="variable"><em>name</em></span></code> or using an expression evaluating to a string as such <code>[<span class="string">"<em>name</em>"</span>]</code>. Local variables are access in the same way.</li>
	
	<li><strong>By rank</strong> : a member can be accessed by its rank i.e. access the i<sup>th</sup> member of the object using an expression evaluating to an integer as such <code>[<span class="number"><em>i</em></span>]</code>.</li>
	
	<li><strong>By object type</strong> : a member can be accessed by its object type using an expression evaluating to an object type as such <code>[<span class="type">className</span>(<em>value_1</em>, <em>value_2</em>...<em>value_k</em>)]</code>.</li>
</ul>
	<p>In case of ambiguity the last member most recently defined will be taken. The access can be chained, using <strong>.</strong> as a delimiter for identifiers. A valid look-up could be for instance:
	<code><span class="variable">array</span>[<span class="number">5</span>].<span class="variable">index</span></code></p>
	
<h4>Reserved variables</h4>

<p>Reserved variables are defined automaticaly for every objects, some are constant and others modifiable. They all begin with <strong>@</strong> :</p>
<ul>
	<li><strong>@size</strong> (modifiable): Represents the size of the object given in bits. Specifying the size in advance is a very important tool for lazy parsing (<a href="#lazy">more on lazy parsing</a>).</li>
	<li><strong>@args</strong> (modifiable): A virtual object representing the parameters values. You can access the i<sup>th</sup> parameter of name <em>param_i</em> either by its rank as so : <code><span class="reserved">@args</span>[<span class="number"><em>i</em></span>]</code> or by its name as so : <code><span class="reserved">@args</span>[<span class="string">"<em>param_i</em>"</span>]</code> or <code><span class="reserved">@args</span>.<span class="variable"><em>param_i</em></span></code>.</li>
	<li><strong>@value</strong> (modifiable): represents the internal value associated with the object. Standard objects such as <code><span class="type">int</span></code>, <code><span class="type">uint</span></code>, <code><span class="type">float</span></code>, <code><span class="type">double</span></code>, <code><span class="type">String</span></code> and <code><span class="type">Bitset</span></code> will have the expected internal values. If you try to evaluate an object as a variable then the value will be called. For instance you can write an integer wrapper as so:</li>
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(<span class="number">32</span>) <span class="variable">payload</span>;
    <span class="reserved">@value</span> = <span class="variable">payload</span>; <span class="comment">//Equivalent to : @value = payload.@value;</span>
}
</pre>
	<li><strong>@info</strong> (modifiable): represents the string that will displayed as a value for the object. If <code><span class="reserved">@info</span></code> is <code><span class="keyword">NULL</span></code> then the value displayed will be the standard representation of <code><span class="reserved">@value</span></code></li>
	
	<li><strong>@parent</strong> (constant): represents the father object. Will be empty if the object is top-level.</li>
	
	<li><strong>@rank</strong> (constant): represents the object rank of the object as a member of its father. Will be <code><span class="number">-1</span></code> if the object is top-level.</li>
</ul>

<h3>Functions</h3>
<p>On top standard operators, functions can be used to transform values or generate new ones. The syntax for a function call is the following :</p>
<pre><span class="function">%<em>function_name</em></span>(<em>value_1</em>, ...,<em>value_n</em>)</pre>
<p>The functions are defined in modules and can be imported in the same way as classes. Function are defined in the top level of file as such:</p>
<pre><span class="keyword">function</span> <span class="function">functionName</span>(<em>param_1</em>, <em>param_2</em> ... <em>param_n</em>) 
{
    <em>statements</em>
}
</pre>
<p>The statements available are the same as the ones used in a class definition except for the declaration. In addition a <strong>return statement</strong> can be used as such:</p>
<pre><span class="keyword">return</span> <em>value</em>;</pre>
<p>If the end of the block is reached with no return statement then the value returned by the function is <code><span class="keyword">NULL</span></code>. By default the parameters are passed by reference, which mean that the value of the parameter can be modified. The keyword <span class="keyword">const</span> can be added in front of the parameter name in the function definition to insure that the value will not be modified. In this case if the value of the parameter is attempted to be modified then the value will be copied and the operation will be done on the copy.</p>
<p>Default values for the parameters can be given as such :</p>
<pre><em>param</em> = <em>value</em></pre>
<p>If not enough parameters are given to a function call, then the default value will be given to the unspecified parameters. If no default parameter has been given then the value will be <code><span class="keyword">NULL</span></code></p>

<h3 id="Inheritance">Inheritance</h3>
	<p>HMscript implements two complementary inheritance models : the extension and the specification.</p>
	<h4>Extension</h4>
		<p>A extends B means that B's parser should be prepended to A's parser. Which mean that defining A as such:</p>
		
<pre><span class="keyword">class</span> <span class="type">B</span>
{
    <em>statement_1</em>
    <em>statement_2</em>
    ...
    <em>statement_k</em>
}

<span class="keyword">class</span> <span class="type">A</span> <span class="keyword">extends</span> <span class="type">B</span>
{
    <em>statement_k+1</em>
    ...
    <em>statement_n</em>
}</pre>
	<p>is equivalent to defining A as such:</p>
<pre><span class="keyword">class</span> <span class="type">A</span>
{
    <em>statement_1</em>
    <em>statement_2</em>
    ...
    <em>statement_n</em>
}</pre>
		<p>The syntax for extension is the following:</p>

<pre><span class="keyword">class</span> <span class="type"><em>className</em></span>(<em>param_1</em>, <em>param_2</em> ... <em>param_n</em>)</span> <span class="keyword">extends</span> <span class="type"><em>parentName</em></span>(<em>value_1</em>, <em>value_2</em> ... <em>value_k</em>)
{
    <em>statements</em>
}</pre>
		<p>The parameter values for the parent object type are expression that can use the parameter values of the children class as variable. As for instance:</p>
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>(size) <span class="keyword">extends</span> <span class="type">Data</span>(<span class="number">32</span>+size)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(size) <span class="variable">payload</span>;
    <span class="reserved">@value</span> = <span class="variable">payload</span>; 
}</pre>
	<h4>Specification</h4>
	<p>A specifies B means that if the type of the object is B then it should be parsed as A. If A specifies B then A should extend B but not necessarely directly. The syntax is :</p>
<pre><span class="keyword">forward</span> <span class="type"><em>parentName</em></span>( ... )</span> <span class="keyword">to</span> <span class="type"><em>childName</em></span>( ... )</pre>
	<p>For convenience, the keyword <span class="keyword">as</span> can be used in class definition :</p>
<table>
<tr>
<td>
<pre><span class="keyword">class</span> <span class="type"><em>A</em></span>(...)</span> <span class="keyword">extends</span> <span class="type"><em>C</em></span>(...) <span class="keyword">as</span> <span class="type"><em>B</em></span>(...)
{
    <em>statements</em>
}</pre>
</td>
<td>
<p>&equiv;</p>
</td>
<td>
<pre><span class="keyword">class</span> <span class="type"><em>A</em></span>(...)</span> <span class="keyword">extends</span> <span class="type"><em>C</em></span>(...)
{
    <em>statements</em>
}

<span class="keyword">forward</span> <span class="type"><em>B</em></span>(...)</span> <span class="keyword">to</span> <span class="type"><em>A</em></span>()</pre>
</td>
</tr>
</table>
<p>and</p>
<table>
<tr>
<td>
<pre><span class="keyword">class</span> <span class="type"><em>A</em></span>(...)</span> <span class="keyword">as</span> <span class="type"><em>B</em></span>(...)
{
    <em>statements</em>
}</pre>
</td>
<td>
<p>&equiv;</p>
</td>
<td>
<pre><span class="keyword">class</span> <span class="type"><em>A</em></span>(...)</span> <span class="keyword">extends</span> <span class="type"><em>B</em></span>( ... )
{
    <em>statements</em>
}
<span class="keyword">forward</span> <span class="type"><em>B</em></span>(...)</span> <span class="keyword">to</span> <span class="type"><em>A</em></span>()</pre>
</td>
</tr>
</table>


<p>For instance, the <span class="type">File</span> object can be specified to define the top-level object of the format.</p>
<p>The specification feature, relies heavily on the fact that the argument values the object type can be changed during the parsing. Therefore a basic format can be for instance implemented as so:</p>
<pre><span class="keyword">class</span> <span class="type">DummyFile</span> <span class="keyword">as</span> <span class="type">File</span>
{
    <span class="keyword">while</span>(<span class="number">1</span>) <span class="type">Container</span> *;
}
 
<span class="keyword">class</span> <span class="type">Container</span>(code)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">code</span>;
    <span class="type">int</span>(<span class="number">32</span>) <span class="variable">size</span>;
    <span class="reserved">@args</span>.code = <span class="variable">code</span>;
    <span class="reserved">@size</span> = <span class="variable">size</span>;
}

<span class="keyword">class</span> <span class="type">VideoContainer</span> <span class="keyword">as</span> <span class="type">Container</span>(<span class="string">"vide"</span>)
{
    ...
}

<span class="keyword">class</span> <span class="type">AudioContainer</span> <span class="keyword">as</span> <span class="type">Container</span>(<span class="string">"audi"</span>)
{
    ...
}

<span class="keyword">class</span> <span class="type">IndexContainer</span> <span class="keyword">as</span> <span class="type">Container</span>(<span class="string">"indx"</span>)
{
    ...
}

...

</pre>

<h3 id="lazy">Lazy parsing</h3>
<p>HexaMonkey relies heavily on lazy parsing, which means that the parsing is done only when necessary. This allows to navigate through large files without having to parse them completely which could take several minutes and take up several gigabytes of memory. Therefore each object is first parsed until its basic information are known, such as its <strong>size</strong>, <strong>type</strong>, <strong>value</strong>  and <strong>showcased values</strong>.</p>
<h4>Knowing the size</h4>
<ul>
<li>
<p><strong>Implicit method for fixed size classes</strong> : For simple classes composed only of a fixed number of members with fixed sizes the size will be computed automaticaly as long as the attribute <code><span class="reserved">@size</span></code> is not modified and that the class does not extend another.</p>
<p>For instance</p>
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>()
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(<span class="number">32</span>) <span class="variable">payload</span>;
}</pre>
<p>The size of object will always be 64 (<code><span class="function">%sizeof</span>(<span class="type">IntWrapper</span>())==<span class="number">64</span></code>) and the two members will be parsed only when asked specifically.</p>

<pre><span class="keyword">class</span> <span class="type">Container</span>(code)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">code</span>;
    <span class="type">int</span>(<span class="number">32</span>) <span class="variable">size</span>;
    <span class="reserved">@args</span>.code = <span class="variable">code</span>;
    <span class="reserved">@size</span> = <span class="variable">size</span>;
}</pre>

<p>Here however the size is modified (as well as the type) and it is therefore necessary to parse the whole object in order to know its size.</p>
</li>
<li>
<p><strong>Explicit method for size defined on construction</strong></p>
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>(size)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(<span class="reserved">@args</span>.size) <span class="variable">payload</span>;
}</pre>
<p>In this case the size is variable, but can be computed on construction. There is two solutions to avoid unnecessary parsing in this case:</p>
It is possible to set the size by setting directly the attribute <code><span class="reserved">@size</span></code>:
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>(size)
{
    <span class="reserved">@size</span> = <span class="number">32</span> + <span class="reserved">@args</span>.size;
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(<span class="reserved">@args</span>.size) <span class="variable">payload</span>;
}</pre>
<p>However here <code><span class="function">%sizeof</span>(<span class="type">IntWrapper</span>(<span class="number">32</span>))==<span class="keyword">NULL</span></code>, which mean that containers such as <code><span class="type">Tuple</span>(<span class="type">IntWrapper</span>(<span class="number">32</span>), <span class="number">32</span>)</code> won't know their size and therefore won't use lazy parsing. The way to insure that the <code><span class="function">%sizeof</span></code> function give the expected results is for the class to extend a class that knows its size on construction such as <code><span class="type">Data</span></code> :</p>
<pre><span class="keyword">class</span> <span class="type">IntWrapper</span>(size) <span class="keyword">extends</span> <span class="type">Data</span>(size+<span class="number">32</span>)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">name</span>;
    <span class="type">int</span>(<span class="reserved">@args</span>.size) <span class="variable">payload</span>;
}</pre>
<p>In this case there will be indeed be <code><span class="function">%sizeof</span>(<span class="type">IntWrapper</span>(<span class="number">64</span>))==<span class="number">64</span></code>.</p>
</li>
<li>
<p><strong>Explicit method for variable size</strong></p>
<p>When the size is variable the size should simply be set as soon as possible by setting the attribute <code><span class="reserved">@size</span></code>. This is the case when the size is specified by a member of the class :

<pre><span class="keyword">class</span> <span class="type">Container</span>(code)
{
    <span class="type">String</span>(<span class="number">4</span>) <span class="variable">code</span>;
    <span class="type">int</span>(<span class="number">32</span>) <span class="variable">size</span>;
    <span class="reserved">@args</span>.code = <span class="variable">code</span>;
    <span class="reserved">@size</span> = <span class="variable">size</span>;
    <em>...</em>
}</pre>

</li>


<?php require_once("footer.php"); ?>
