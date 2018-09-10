% jgaa-art.cls version 1, 11/2008
%
% LaTeX class for articles published in the Journal of Graph
% Algorithms and Applications, http://jgaa.info
%
% Based on previous package jga-article.sty file


\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{jgaa-art}[2012/07/17 v2.0 LaTeX class for the Journal of Graph Algorithms and Applications]

\LoadClass[twoside]{article}

\RequirePackage{hyperref}
\RequirePackage{fancyhdr}
\RequirePackage{graphicx}

\setlength{\oddsidemargin}{62pt}
\setlength{\evensidemargin}{62pt}
\setlength{\marginparwidth}{65pt}

\newcommand{\ShortIssueNo}{vol.\ 0, no.\ 0, 0000}
\newcommand{\IssueNo}{vol.\ 0, no.\ 0, 0000}
\newcommand{\Issue}[5]{%
  \renewcommand{\ShortIssueNo}{#1(#2) #3--#4 (#5)}
  \renewcommand{\IssueNo}{vol.\ #1, no.\ #2, pp.\ #3--#4 (#5)}
  ~\vspace{-0.5in}
  \hrule
  \begin{center}
    \parbox{1.4in}{ \includegraphics[scale=0.2]{cube-spread} }
    \parbox{3.25in}{%
      {\large\sf Journal of Graph Algorithms and Applications} \\
      {\normalsize\sf http://jgaa.info/} {\normalsize\IssueNo} \\
      {\footnotesize \sf \href{http://dx.doi.org/\DOIInfo}{DOI: \DOIInfo}} 
    }
  \end{center}
  \hrule
  \vspace{6pt}
  \setcounter{page}{#3}
  }
  
\newcommand{\DOIInfo}{}
\newcommand{\doi}[1]{\renewcommand{\DOIInfo}{#1}}  
  
  
\newcommand{\HeadingAuthorInfo}{}
\newcommand{\HeadingAuthor}[1]{\renewcommand{\HeadingAuthorInfo}{#1}}
\newcommand{\HeadingTitleInfo}{}
\newcommand{\HeadingTitle}[1]{\renewcommand{\HeadingTitleInfo}{#1}}


\fancyhead{} % clear all fields
\fancyhead[LE]{\thepage~~~~\textrm{\HeadingAuthorInfo}~~\textit{\HeadingTitleInfo\/}}
\fancyhead[RO]{ {\sf JGAA}, \ShortIssueNo~~~~\thepage}
\fancyfoot[C]{}
\renewcommand{\headrulewidth}{0.0pt}
\renewcommand{\footrulewidth}{0.0pt}

\newtoks\auth@toks
\newtoks\authpdf@toks
\newtoks\mail@toks
\newtoks\affil@toks
\newtoks\title@toks
\newtoks\titlepdf@toks
\newtoks\history@toks
\newtoks\editor@toks
\newtoks\ack@toks


\newcounter{affil@count}
\renewcommand{\theaffil@count}{\textsuperscript{\arabic{affil@count}}}

\newcounter{author@count}
\setcounter{author@count}{0}

\renewcommand{\author}[3][]{%
  \if!#1!%
    \auth@toks=\expandafter{\the\auth@toks\@author{#2}}%
    \mail@toks=\expandafter{\the\mail@toks\@email{#2}{#3}}%
  \else%
    \auth@toks=\expandafter{\the\auth@toks\@authoraff{#1}{#2}}%
    \mail@toks=\expandafter{\the\mail@toks\@email{#2}{#3}}%
  \fi%
  \ifnum \value{author@count} > 0%
        \authpdf@toks=\expandafter{\the\authpdf@toks{, #2}}%
    \else%
        \authpdf@toks=\expandafter{\the\authpdf@toks{#2}}%
    \fi%
    \stepcounter{author@count}%
}

\newcommand{\affiliation}[2][]{%
  \ifnum\c@affil@count<0
    \ClassError{jgaa-article}{Two affiliations given without optional argument}{}%
  \else
    \if!#1!%
      \affil@toks={\@oneaffil{#2}}%
      \c@affil@count=-1
    \else
      \affil@toks=\expandafter{\the\affil@toks\@affil{#1}{#2}}%
      \refstepcounter{affil@count}\label{#1}%
    \fi
  \fi
}

\renewcommand{\title}[1]{
  \title@toks=\expandafter{\the\title@toks\@title{#1}}
  \titlepdf@toks={#1}
  }

\renewcommand{\@title}[1]{
    \begin{center}
        \Large\bf \par#1
    \end{center}
    }    

\renewcommand{\@author}[1]{
    {\large{\emph{#1}}}
}
  
\newcommand{\@authoraff}[2]{
    {\large{\emph{#2$\,$}}\formataffilmark{#1}}
}

\newcommand\@marksep{,}
\newcommand\displaysep{$^,$}
\newcommand\refentry[1]{\ref{#1}}


\newcommand\formataffilmark[1]{%
  \let\@marksep\@refsepinit
  \@for\@ii:=#1\do{%
    \@marksep
    \refentry{\@ii}%
  }%
}
\def\@refsepinit{\let\@marksep\displaysep}

\newcommand{\@email}[2]{
    \footnotesize{\href{mailto:#2}{\textsf{#2}} (#1)}
}

\newcommand{\@oneaffil}[1]{
\centering%
  {\par#1}%
}%

\newcommand{\@affil}[2]{%
\centering%
  {\par\ref{#1}#2}%
}%

\newcounter{history@count}
\setcounter{history@count}{0}

\newcommand{\submitted}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Submitted}{#1}}%
}

\newcommand{\reviewed}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Reviewed}{#1}}%
}

\newcommand{\revised}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Revised}{#1}}%
}

\newcommand{\accepted}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Accepted}{#1}}%
}

\newcommand{\final}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Final}{#1}}%
}

\newcommand{\published}[1]{
        \stepcounter{history@count}%
        \history@toks=\expandafter{\the\history@toks\@history{Published}{#1}}%
}

\newcommand{\type}[1]{
        \stepcounter{history@count}%
        \editor@toks=\expandafter{\the\editor@toks\@editor{Article type}{#1}}%
}

\newcommand{\editor}[1]{
        \stepcounter{history@count}%
        \editor@toks=\expandafter{\the\editor@toks\@editor{Communicated by}{#1}}%
}

\newcommand{\@history}[2]{
\parbox[c][0.7cm]{2cm}{\begin{center}\footnotesize{\textsf{#1: }\par #2}\end{center}}
}

\newcommand{\@editor}[2]{
\parbox[c][0.7cm]{2.5cm}{\begin{center}\footnotesize{\textsf{#1: }\par #2}\end{center}}
}

\newcommand{\Ack}[1]{%
    \ack@toks=\expandafter{\the\ack@toks\@ack{#1}}%
}%

\newcommand{\@ack}[1]{%
    {\footnotesize{#1}}%
}%

\def\printauthors{\the\auth@toks}
\def\printaffiliations{\the\affil@toks}
\def\printtitle{\the\title@toks}
\def\printhistory{\the\history@toks}
\def\printeditor{\the\editor@toks}
\def\printack{\the\ack@toks}
\def\printmail{%
    \ifnum \value{author@count} > 1%
        {\footnotesize{\emph{E-mail addresses:}}\the\mail@toks}%
    \else%
        {\footnotesize{\emph{E-mail address:}}\the\mail@toks}%
    \fi%
}%

\newcommand{\history}{
\ifnum \value{history@count} > 0%
    \vfill%
    \begin{center}%
    \fbox{\parbox{0.98\textwidth}{\centering \printhistory\\%
    \printeditor}}%
    \end{center}%
\fi%
\vfill%
\hrule%
\vspace{1mm}%
\printack%

\noindent\printmail%
}%


\renewcommand{\maketitle}{%
\hypersetup{pdftitle=\the\titlepdf@toks,pdfauthor=\the\authpdf@toks}
\printtitle%
\begin{center}%
\printauthors%
\vspace{5mm}%
\printaffiliations%
\vspace{5mm}%
\end{center}%
}%
  
\newcommand{\Body}{%
    \history%
    \thispagestyle{empty} %
    \clearpage\pagestyle{fancy}%
}%
  
\newtheorem{lemma}{Lemma}
\newtheorem{theorem}{Theorem}
\newenvironment{proof}{\par\addvspace\topsep\noindent
{\bf Proof:} \ignorespaces }{\qed}
\newcommand{\qed}{\hspace*{\fill}$\Box$\ifmmode\else\par\addvspace\topsep\fi}
