# arc-02-my_tar
<div class="card-block">
<div class="row">
<div class="col tab-content">
<div class="tab-pane active show" id="subject" role="tabpanel">
<div class="row">
<div class="col-md-12 col-xl-12">
<div class="markdown-body">
<p class="text-muted m-b-15">
</p><h1>My Tar</h1>
<p>Remember to git add &amp;&amp; git commit &amp;&amp; git push each exercise!</p>
<p>We will execute your function with our test(s), please DO NOT PROVIDE ANY TEST(S) in your file</p>
<p>For each exercise, you will have to create a folder and in this folder, you will have additional files that contain your work. Folder names are provided at the beginning of each exercise under <code>submit directory</code> and specific file names for each exercise are also provided at the beginning of each exercise under <code>submit file(s)</code>.</p>
<hr>
<table>
<thead>
<tr>
<th>My Tar</th>
<th></th>
</tr>
</thead>
<tbody>
<tr>
<td>Submit directory</td>
<td>.</td>
</tr>
<tr>
<td>Submit files</td>
<td>Makefile - *.c - *.h</td>
</tr>
</tbody>
</table>
<h3>Description</h3>
<h1>my_tar</h1>
<h2>Command Name</h2>
<p><code>my_tar</code></p>
<h2>Description</h2>
<p>my_tar is a command to manipulate tape archive. The first option to tar is a mode indicator from the following list:</p>
<ul>
<li>-c Create a new archive containing the specified items.</li>
<li>-r Like -c, but new entries are appended to the archive. The -f option is required.</li>
<li>-t List archive contents to stdout.</li>
<li>-u Like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. The -f option is required.</li>
<li>-x Extract to disk from the archive. If a file with the same name appears more than once in
the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies.</li>
</ul>
<p>In -c, -r, or -u mode, each specified file or directory is added to the archive in the order specified on the command line. By default, the contents of each directory are also archived.</p>
<p>Unless specifically stated otherwise, options are applicable in all operating modes:</p>
<ul>
<li>
<code>-f file</code> Read the archive from or write the archive to the specified file. The filename can be standard input or standard output.</li>
</ul>
<p>my_tar will not handle file inside subdirectory.</p>
<h2>Exit Status</h2>
<p>The tar utility returns 0 on success, and &gt;0 if an error occurs.</p>
<h2>Hints</h2>
<ul>
<li>https://www.gnu.org/software/tar/manual/html_node/Standard.html</li>
</ul>
<h2>Technical Information</h2>
<ol>
<li>You must create a Makefile, and the ouput is the command it self</li>
<li>You can use (the number is the man page):</li>
</ol>
<ul>
<li>chmod</li>
<li>close|(2)</li>
<li>free(3)</li>
<li>fstat(2)</li>
<li>getpwuid</li>
<li>getgrgid</li>
<li>getxattr</li>
<li>listxattr</li>
<li>lseek(2)</li>
<li>major</li>
<li>malloc(3)</li>
<li>minor</li>
<li>mkdir(2)</li>
<li>open(2)</li>
<li>opendir</li>
<li>read(2)</li>
<li>readdir</li>
<li>readlink</li>
<li>stat(2)</li>
<li>lstat(2)</li>
<li>symlink</li>
<li>unlink(2)</li>
<li>time</li>
<li>ctime</li>
<li>utime</li>
<li>write(2)</li>
</ul>
<ol start="3">
<li>
<p>No other functions or system calls are allowed.</p>
</li>
<li>
<p><strong>Global variables are strictly FORBIDDEN</strong></p>
</li>
</ol>
<h2>Example</h2>
<p>The following creates a new archive called file.tar that contains two files source.c and
source.h:</p>
<pre class=" language-shell"><code class=" language-shell"><span class="token operator">&gt;</span> ./my_tar -cf file.tar source.c source.h
</code></pre>
<p>To view a detailed table of contents for this archive:</p>
<pre class=" language-shell"><code class=" language-shell"><span class="token operator">&gt;</span> ./my_tar -tf file.tar
source.c
source.h
</code></pre>
<ol start="5">
<li>Errors handling
Errors will be written on STDERR.</li>
</ol>
<p>File not found (provided file is: i_don_t_exist):
my_tar: i_don_t_exist: Cannot stat: No such file or directory</p>
<p>Error with the tarball file (provided file is: tarball.tar):
my_tar: Cannot open tarball.tar</p>

<p></p>
</div>

</div>
</div>
</div>
<div class="tab-pane" id="resources" role="tabpanel">
</div>
</div>
</div>
</div>
