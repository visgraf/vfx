////////////////////////////////////////////////////////////////////////////// 
//
// ‐‐‐ LoadProgram.c ‐‐‐ 
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h> 


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h> 
#else
#include <GL/glew.h> 
#endif 

#include "loadprogram.h"

void ResetError_GL() 
{
  while (glGetError() != GL_NO_ERROR ) 
    ;
}

void CheckError_GL( char *msg , GLuint prog, GLuint shdr) 
{
  GLenum error;

  while ((error = glGetError()) != GL_NO_ERROR ) {
    fprintf(stderr, "(%s) %s [%d,%d] : %s\n", "gl error", msg, prog, shdr, gluErrorString(error));
  }
}


/*‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐*/

GLuint LoadProgram_GL( const char* vShader, const char* fShader )
{
  GLuint shader = 0, program = 0; 
  GLint completed;

  program = glCreateProgram();

  /*‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
  ** 
  ** ‐‐‐ Load and compile the vertex shader ‐‐‐ 
  */
  
  if ( vShader != NULL ) { 
    shader = glCreateShader( GL_VERTEX_SHADER ); 
    glShaderSource( shader, 1, &vShader, NULL ); 
    glCompileShader( shader ); 
    glGetShaderiv( shader, GL_COMPILE_STATUS, &completed );
  }
  if ( !completed ) { 
    GLint len; 
    char* msg;

    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
    msg = (char*) malloc( len ); 
    glGetShaderInfoLog( shader, len, &len, msg ); 
    fprintf( stderr, "Vertex shader compilation failure:\n%s\n", msg ); 
    free( msg );

    glDeleteProgram( program ); 
    exit( EXIT_FAILURE );
  } 
  glAttachShader( program, shader ); 
  CheckError_GL("attach vert shader", program, shader);

  /*‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐ 
  **
  ** ‐‐‐ Load and compile the fragment shader ‐‐‐	
  */

  if ( fShader != NULL ) { 
    shader = glCreateShader( GL_FRAGMENT_SHADER ); 
    glShaderSource( shader, 1, &fShader, NULL );
    glCompileShader( shader ); 
    glGetShaderiv( shader, GL_COMPILE_STATUS, &completed );

    if ( !completed ) { 
      GLint len;
      char* msg;
      
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len ); 
      msg = (char*) malloc( len ); glGetShaderInfoLog( shader, len, &len, msg );
      fprintf( stderr, "Fragment shader compilation failure:\n%s\n", msg );
      free( msg );

      glDeleteProgram( program ); 
      exit( EXIT_FAILURE );
    } 
    glAttachShader( program, shader ); 
    CheckError_GL("attach frag shader", program, shader);
  }

  /*‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
  **
  ** ‐‐‐ Link program ‐‐‐ 
  */
  glLinkProgram( program );
  glGetProgramiv( program, GL_LINK_STATUS, &completed );

  if ( !completed ) { 
    GLint len;
    char* msg;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len ); 
    msg = (char*) malloc( len ); glGetProgramInfoLog( program, len, &len, msg ); 
    fprintf( stderr, "Program link failure:\n%s\n", msg );
    free( msg ); 
    glDeleteProgram( program );
    exit( EXIT_FAILURE );
  }
  return program;
}


