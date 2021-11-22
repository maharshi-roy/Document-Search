import React from 'react';
import { TextField, withStyles } from '@material-ui/core';
import { red } from '@material-ui/core/colors';

const styles = theme => ({
    root: {
        margin: 'auto',
        flex: 3
    }
});

export default withStyles(styles)(({ label, value, handleText=()=>{}, classes, ...restProps }) => 
    <TextField
        className={classes.root}
        label={label}
        value={value}
        onChange={(event) => handleText(event.target.value)}
        InputLabelProps={{
            style: {
                color: red[400]
            }
        }}
        {...restProps}
    />
);