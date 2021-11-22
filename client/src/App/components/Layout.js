import React, { Fragment } from 'react';
import { withStyles, LinearProgress } from '@material-ui/core';
import { amber } from '@material-ui/core/colors';

const styles = theme => ({
    header: {
        height: 40,
        backgroundColor: amber['800'],
        marginBottom: 20,
    },
    footer: {
        flex: 1,
        marginTop: 20,
    },
    progress: {
        top: 40
    }
});

export default withStyles(styles)(({ classes, children, indexing }) => (
    <Fragment>
        <div className={classes.header}>
            {indexing && <LinearProgress className={classes.progress} />}
        </div>
        {children}
        <div className={classes.footer} />
    </Fragment>
));