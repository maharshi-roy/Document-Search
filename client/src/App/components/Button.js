import React from 'react';
import { Button, withStyles } from '@material-ui/core';
import { amber, grey } from '@material-ui/core/colors';

const styles = theme => ({
    container: {
        display: 'flex',
        flexDirection: 'column',
    },
    topPadding: {
        flex: 1
    },
    btn: {
        flex: 3,
        backgroundColor: amber[800],
        '&:hover': {
            backgroundColor: grey[100],
            color: amber[800],
        }
    },
    root: {
        height: '40%',
        flex: 1,
        marginBottom: 0,
        
    }
});

export default withStyles(styles)(({callFunc, label, classes}) => 
    <div className={classes.container}>
        <div className={classes.topPadding} />
        <Button
            className={classes.btn} 
            onClick={callFunc}
        >
            {label}
        </Button>

    </div>
);