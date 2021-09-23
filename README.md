# Progetto di Algoritmi e Strutture Dati 2020 (Prova Finale AA 2019-2020)

## General Description

The development of this project is part of Algoritmi e Principi dell'Informatica course at Politecnico di Milano.

The purpose of the project is the implementation with C language of edU text editor (or ed with multiple Undo/Redo) optimizing running time and memory usage.

The interface of the text editor is inspired by the traditional editor [ed](https://en.wikipedia.org/wiki/Ed_(text_editor))

## Command Description
* **(add1,add2)c** This command changes the lines between add1 and add2 (included). The following text must be composed by add2-add1+1 lines. add1 must be present in the text or must be the first line after the last line of the text;
* **(add1,add2)d** This command deletes the lines between add1 and add2 (included) shifting lines after add2 if they are present. The deletion of the lines that don't exist, don't affect anything.
* **(add1,add2)p** This command prints the lines between add1 and add2 (included) if they are present. If there isn't any text in the selected lines the editor prints a '.';
* **(number)u** The Undo command cancels a number of commands (c or d) equal to the number specified in parentheses (where the number is an integer greater than zero). A sequence of consecutive Undo commands cancels a number of steps equal to the sum of the steps specified in each of them. If the number of the commands to be canceled is higher than the number of exectued commands, all steps are cancelled. The execution of a text modification command (c or d),after an Undo, cancels the effect of the commands definitively canceled. Note that in the number of commands to be cancelled are also counted the commands that have no effect( for example the delation of a block of lines that do not exist).
* **(number)r** The Redo command cancels the effect of the Undo command for a number of commands equal to the number specified. If this command is called consecutive times, it follows the same logic of the Undo command.
* **q** This command quits the editor.




## Project Specification

The project specification with examples are available [here](https://github.com/stefanofossati/Progetto-API-2020/blob/main/specs/Project%20Specifiaction%20AA%202019-2020.pdf) (ITA)
