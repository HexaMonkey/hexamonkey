<?php require_once("head.php"); ?>
<?php require_once("navigation.php"); ?>
<?php require_once("header.php"); ?>

<h2>HexaMonkey Architecture</h2>

<h3>General Architecture</h3>

<img src="HexamonkeyGlobal.png" alt="Architecture Overview" width="75%"/>

<h3>The compiler</h3>

<p>The compiler is in charge of converting the HMSripts text files (.hm) into
compiled binary files (.hmc), using the tools Flex and Bison in charge of lexing
and parsing. The compiler outputs in a special format that implements the EBML
specifications. It must also provide to
the core the model.h file, which contains information on the syntax model of a
HMScript.</p>

<h3>The core</h3>

<p>The main goal of the core is to extract a structured data tree from a binary
file, using <a href="doc/classModule.html">Modules</a>.</p>

<p>The main class in the core is the <a href="doc/classModuleLoader.html">ModuleLoader</a>
which assigns <a href="doc/classModule.html">Module</a> to <a href="doc/classFile.html">File</a>
to parse them. The modules are responsible for generating <a href="doc/classParser.html">Parser</a>
that decompose a file into tree structure with the nodes being instances of the class Object. Modules
can be imported into others so that generic parsers can be used by several Module
such as integers or strings.</p>

<p>The Module can either be native or HMScript. The native modules are subclasses
of the virtual Module class. These include the default module, which is imported by every module
automatically and provides basic structures such as the basic file, the array... and the standard
module which provides basic atoms such as integers, floats, strings..., this module exist under two
versions : the little endian version (codename lestd) and the big endian version (codename). To create
a native module the most pratical class to reimplement is the MapModule. However it is recommended
when possible to write a new module as a HMScript.</p>

<h4>1. Loading native modules</h4>

<p>The first thing to do is to import the mandatory modules, through the
<a href="doc/classModuleLoader.html">ModuleLoader</a>. We give a list of the
native modules that will be used for the further parsing.</p>

<pre><span class="type">ModuleLoader</span> moduleLoader;
moduleLoader.addModule(<span class="string">"hmc"</span>,   <span class="keyword">new</span> <span class="type">HmcModule</span>(getFile(modelsDirs, <span class="string">"hmcmodel.csv"</span>)));
moduleLoader.addModule(<span class="string">"ebml"</span>,  <span class="keyword">new</span> <span class="type">EbmlModule</span>);
moduleLoader.addModule(<span class="string">"mymodule"</span>, <span class="keyword">new</span> <span class="type">MyModule</span>(<span class="keyword">true</span>));
...</pre>

<p>By doing this, the ModuleLoader will keep a map linking the string "mymodule"
to the class MyModule. </p>


<h4>2. Accessing modules</h4>

<p>Later on, a module could be accessed using :</p>

<pre>moduleLoader.getModule(<span class="string">"mymodule"</span>);</pre>

<p>This will return a MyModule object, but will also import in it every module
dependency MyModule has.</p>

<p>One can also get the appropriate module for a file:</p>

<pre><span class="keyword">const</span> <span class="type">Module</span>& module = moduleLoader.getModule(file);</pre>

<p>Internal : each module in the moduleLoader can contain one or many
<a href="doc/classFormatDetector.html">FormatDetectors</a>, that will either
look for specific file extension, magic number (byte sequence at the
beginning of the file) or syncbyte (periodical occurence of a byte sequence in
the file, e.g. 'G' in MPEG-TS). The first module with a matching format detector
will be returned.</p>

<p>Theorically, this could be enough to interpret data, if each Modules
corresponding to a file type (png, gif, etc...) was implemented. However, most
of them are described using HMScripts (see part 4).</p>


<h4>3. Parsing a data file</h4>

<p>Once the module corresponding to a file has been loaded, the file may be
parsed using</p>

<pre><span class="type">Object</span>* obj = module.handle(<span class="type">defaultTypes::file</span>, file);</pre>

<p>Internal : here, the new <a href="doc/classObject.html">Object</a> is given all
the Parsers necessary to parse itself, but it has not been parsed yet. An Object
is a tree structure : it contains a list of pointers to other Objects (its node).
Since it is not parsed, initially this list is empty.</p>

<p>Once all this has been done, one may extract data from the Object using
either access, lookingForType or lookUp. For instance:</p>

<pre>object.lookUp(child_name, forceParse);</pre>

<p>looks in the already available childs, and returns the child of name
child_name (if found). If it is not found, the boolean forceParse will determine
if the Object should continue to parse itself until child_name is found, or
simply returns a null pointer.</p>


<h4>4. Loading modules from HMScripts</h4>

<p>To create modules from HMScripts, we must call the
<a href="doc/classProgramLoader.html">ProgramLoader</a> :</p>

<pre><span class="type">ProgramLoader</span> programLoader(static_cast&lt;const <span class="type">HmcModule</span> &amp;&gt;(moduleLoader.getModule(<span class="string">"hmc"</span>)), compilerDirs, userDir);
moduleLoader.setDirectories(scriptsDirs, programLoader);</pre>

<p>Internal : Here, each HMScript found int scriptDirs is re-compiled (if there were any change since
last compilation). The compiled files are put in userDir and then parsed using the
HmcModule, entirely (no lazy parsing).
This will output a <a href="doc/classProgram.html">Program</a> per HMScript. which has
in its underlying structure an Object (tree), that contains all relevant
information for its file type (tree structure).</p>

<p>Finally, those programs are stored in the moduleLoader as
<a href="doc/classFromFileModule.html">FromFileModules</a>,
with the prefix of the hmscript name as the map key. (e.g. png.hm will give
access to a module at the key <span class="string">"png"</span>).</p>

<p>The HMScript language can
also be used on the fly to evaluate expression. This is for instance used by the Filter class.</p>

<h4>5. BlockExecution, Evaluator and Scopes</h4>

<p>Not only does a HMScript contains the structure of the file, it also ships
dynamic langage that make it possible to create models of files that may have a
dynamic structure (through conditional branching, loops, etc...). This is dealt
with the <a href="doc/classBlockExecution.html">BlockExecution</a>
<a href="doc/classEvaluator.html">Evaluator</a> and
<a href="doc/classScope.html">Scope</a> classes. Those class are therefore
specific to HMScript, they are not used in native modules.</p>


<h5>a. BlockExecution</h5>

<p>A BlockExecution represents a succession of instructions which may be :
<ul>
    <li>declarations : creating a new child node to the Object</li>
    <li>right values, involving variable data, but not creating a new child node
    to the Object</li>
    <li>loops, themselves containing a BlockExecution</li>
    <li>etc...</li>
</ul>
</p>

<p>For instance, each class definition contains a BlockExecution :</p>

<pre><span class="keyword">class</span> <span class="type">A</span> {
    <span class="type">Data</span>(<span class="number">8</span>) a;
    <span class="type">int</span>(<span class="number">8</span>)  b;
}</pre>

<p>Here, the class definition contains a BlockExecution that itself contains two
declarations.</p>

<p>When a file needs to be parsed, the BlockExecution will be called, and will
execute its instructions, until it has parsed enough or encounters a break point
(break or continue in a while loop).</p>

<p>Every BlockExecution owns a Scope and an Evaluator.</p>

<h5>b. Scope</h5>

<p>A Scope is basically a way to determined what information (objects,
variables) should be accessible in a BlockExecution. In the case of a class, for
instance, the Scope will be composite, containing an
<a href="doc/classObjectScope.html">ObjectScope</a> (to access the
children, parent of the Object), and a <a href="doc/classLocalScope.html">
LocalScope</a> (containing all the variables declared in the class).</p>


<h5>c. Evaluator</h5>

<p>In case of rightValues instructions, the BlockExecution calls the Evaluator.
It will be in charge of computing arithmetic operations, asigning values to , calling
functions.</p>

<p>Example :</p>

<p>When a BlockExecution encounters:</p>

<pre><span class="keyword">var</span> bytesize = <span class="number">8</span> * bitsize;</pre>

<p>it will first call the Evaluator to evaluate 8 * bitsize. The evaluator will
try to find a variable or object called bitsize in its scope. It will then apply
the multiplication, find the variable bytesize in the scope and assign the
result to it.</p>

<?php require_once("footer.php"); ?>
