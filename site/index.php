<?php require_once("head.php"); ?>
<?php require_once("navigation.php"); ?>
<?php require_once("header.php"); ?>
			<h2>About</h2>
			
			<p>HexaMonkey is a freely distributed framework which goal is to provide tools to view the internal structure of any binary file and navigate through it seamlessly. It can be used to facilitate the development and debugging of encoding and decoding software.</p>
			<p>It relies on lazy and asynchronous parsing to offer a pleasant experience to the user. It is highly exentensible thanks to its modular architecture and a powerful scripting langage : HMscript.<p>
			<p>The format already supported are :
			<ul> 
				<li>Video :
					<ul>
						<li>mpeg4</li>
						<li>mpeg2-ts</li>
						<li>matrovska</li>
						<li>avi</li>
						<li>asf/wmw (incomplete)</li>
					</ul>
				</li>
				<li>Image :
					<ul>
						<li>png</li>
						<li>jpeg (incomplete)</li>
					</ul>
				</li>
				<li>Archive :
					<ul>
						<li>zip</li>
					</ul>
				</li>
			</ul>
			<p>We strongly encourage you to implement other formats and share them : see the HMScript <a href="hmdoc.php">documentation</a> for more information.</p>
			<div style="display:table;margin:auto"><img src="media/screenshot.png"/></div>
<?php require_once("footer.php"); ?>
