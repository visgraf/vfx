# vfx
Code base for the book <a href="https://www.amazon.com/Introduction-Visual-Effects-Computational-Approach-ebook/dp/B0BJYBHMDP/ref=sr_1_1?keywords=introduction+to+visual+effects&qid=1673658651&sr=8-1">Introduction to Visual Effects</a>. It implements: Matchmove, Global Illumination (Pathtracing), and is prepared for making Image Based Lighting in combination of the LuminanceHDR Software. As a result, it can generate simple realistic visual effects.

Result Example
-------------------------
<p align="center"><img src="demo.png"></center></p>
<p align="center">Two virtual spheres over a captured video.</p>

The complete video can be watched <a href="https://youtu.be/0dFbJLH55wE">here</a>.

Visual Effects Software
=======================

This is the companion software package of the book  <a href="https://www.amazon.com/Introduction-Visual-Effects-Computational-Approach-ebook/dp/B0BJYBHMDP/ref=sr_1_1?keywords=introduction+to+visual+effects&qid=1673658651&sr=8-1">
"Introduction to Visual Effects: A computational approach"</a> 
by Bruno Madeira and Luiz Velho.

The software contains an implementation of all the
algorithms described in the book.
It is splitted into two parts: the matchmove software
and the s3d-ptrace software.

The matchmove module has the Computer Vision routines
necessaries for making a successful matchmove procedure. 
The math processing of this module is solved using the 
Gnu Scientific Library.

The s3d-ptrace module has the Computer Graphics routines necessaries
for implementing a Pathtracing and Image Based Lighting processing.
The functions of this module uses as basis the software of the book:
"Design and Implementation of 3D Graphics Systems" from: Luiz Velho,
Jonas Gomes and Mario Costa Sousa.  

Installation and Testing
----------------------------

The instructios for intalling and testing these modules are presented
in the README files inside the directories: ./matchcmove and ./s3d-ptrace.

Interdependence of the Modules
------------------------------

The scripts of this software have interdependence between them.
In order to simplifying the process of testing, the
input files necessary for running almost all the scripts are available.
The unique exception are the dependence of 
./s3d-ptrace/demo/d2, ./s3d-ptrace/demo/d3 and ./s3d-ptrace/demo/d4
 which depend to the execution of ./s3d-ptrace/demo/d0 before.  
Using the scripts defined on this package the user can generates
 the video ./video/demo.mp4.
 
Copyright 2022 - Bruno Madeira & Luiz Velho
