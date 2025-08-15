# Basic cell automat "Conway's game of life"
<p>Implementation of cell automat "Conway's game of life" invented by John Horton Conway in 1970.</p>
<p>It is made purely for experimentation purposes as to test capabilities of SDL libraries and purely functional programming paradigm. As to conform to functional programming style, all code is written mostly on pure C.</p>
<p>Game of life is one of the examples of system emergent properties. Once the initial state is set
all cells will act according to preset rules. This behaviour can produce various 'configurations'
that will have different behaviours, such as moving in particular direction, producing other 'configurations'
or deleting certain 'configurations'.</p>
<p><b>To successfully build this application through MSVS:</b></p>
<ol>
  <li>Clone repositories <a href="https://github.com/libsdl-org/SDL">SDL</a> and <a href="https://github.com/libsdl-org/SDL_ttf">SDL_ttf</a> into your C:\ directory.</li>
  <li>Make two environmental variables named 'SDL_HOME' and 'SDLTTF_HOME' and set them as your SDL and SDL_ttf repositories directories respectively.</li>
</ol>
<p>Only after doing this you can open your MSVS solution. Also, SDL_ttf is dependent of compiled SDL libraries, so you need to open its project properties and set value for additional linking libraries as '$(SolutionDir)$(Platform)\$(Configuration)' for all configurations.</p>
<p>If you have any troubles with project be sure to contact me.</p>
