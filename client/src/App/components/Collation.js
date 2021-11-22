import React, { Fragment } from 'react';
import { withStyles, Typography } from '@material-ui/core';
import Table from './Table';

const styles = theme => ({
    root: {
    },
    eachResult: {
        marginTop: 20
    },
    context: {
        position: 'relative',
        left: 50,
    }
});

export default withStyles(styles)(({ data, columns, classes }) => (
    <div className={classes.root}>
        {Object.keys(data).map((key,index) => (
            <div key={index} className={classes.eachResult}>
                <Typography variant="h6" className={classes.context}>
                    {key}
                </Typography>
                <Table rows={data[key]} columns={columns} />
            </div>
        ))}
    </div>
));