import React from 'react';
import { Table, TableHead, TableBody, TableRow, withStyles, TableCell } from '@material-ui/core';
import { amber } from '@material-ui/core/colors';
import clsx from 'clsx';

const styles = theme => ({
    table: {
        width: '90%',
        margin: 'auto',
        marginTop: 20,
    },
    cells: {
        textAlign: 'center'
    },
    head: {
        backgroundColor: amber[800],
    },
    headRow: {

    },
    headCell: {
        color: 'black'
    },
    body: {

    },
    bodyRow: {

    },
    bodyCell: {
        backgroundColor: 'white',
    },
});

export default withStyles(styles)(({ rows, columns, classes }) => (
    <Table className={classes.table}>
        <TableHead className={classes.head}>
            <TableRow className={classes.headRow}>
                {columns.map((cell,index) => 
                    <TableCell className={clsx(classes.cells,classes.headCell)} key={index}>{cell}</TableCell> 
                )}
            </TableRow>
        </TableHead>
        <TableBody className={classes.body}>
            {rows.map((row,rowIndex) => 
            <TableRow key={rowIndex} className={classes.bodyRow}>
                {row.map((cell,cellIndex) => 
                    <TableCell key={cellIndex} className={clsx(classes.cells,classes.bodyCell)}>{cell}</TableCell>    
                )}
            </TableRow>
            )}
        </TableBody>
    </Table>
));