<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY dbstyle SYSTEM "docbook.dsl" CDATA DSSSL>
]>

<style-sheet>
<style-specification use="docbook">
<style-specification-body>

;; your stuff goes here...
(define nochunks
    ;; Suppress chunking of output pages
#f)
(define html-index-filename
        ;; Name of HTML index file
   "index.html")

(define %html-ext%
    ;; Default extension for HTML output files
       ".html")
  
(define %root-filename%
    ;; Name for the root HTML document
       "d1")

(define use-output-dir
    ;; If an output-dir is specified, should it be used?
       #t)
(define %output-dir%
    ;; The directory to which HTML files should be written
       "html-out")
(define %css-decoration%
    ;; Enable CSS decoration of elements
       #t)

(define %number-programlisting-lines%
	#t)

(define %default-language% "es")
;(define %gentext-language% #t)

  
</style-specification-body>
</style-specification>
<external-specification id="docbook" document="dbstyle">
</style-sheet>
