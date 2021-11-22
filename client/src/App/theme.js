import { createMuiTheme } from '@material-ui/core/styles';

export default createMuiTheme({
    typography: {
        useNextVariants: true,
    },
    palette: {
        primary: { main: '#90a4ae' },
        secondary: { main: '#ff5252' },
        error: { main: '#e62739' }
    }
});
//grey: '#e1e8f0',