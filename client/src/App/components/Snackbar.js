import React from 'react'
import { Snackbar, withStyles, SnackbarContent, IconButton } from '@material-ui/core';
import { Close as CloseIcon, CheckCircle as CheckCircleIcon, Error as ErrorIcon } from '@material-ui/icons';
import { green, red } from '@material-ui/core/colors';

const styles = theme => ({
    success: {
        backgroundColor: green[600]
    },
    error: {
        backgroundColor: red[500]
    },
    message: {
        display: 'flex',

        '& > *': {
            paddingRight: 5
        }
    }
});

export default withStyles(styles)(({ open, handleClose, type, classes }) => 
    <Snackbar
        anchorOrigin={{
            vertical: 'bottom',
            horizontal: 'left',
        }}
        open={open}
        autoHideDuration={2000}
        onClose={handleClose}
    >
        <SnackbarContent
            className={((type.variant=='success' && classes.success) ||
                        (type.variant=='error' && classes.error))}
            message={
                <span className={classes.message}>
                    {type.variant == 'success' && <CheckCircleIcon />}
                    {type.variant == 'error' && <ErrorIcon />}
                    <div style={{ margin: 'auto'}}>
                        {type.message}
                    </div>
                </span>
            }
            action={
                <IconButton onClick={handleClose}>
                    <CloseIcon />
                </IconButton>
            }
        />
    </Snackbar>
);