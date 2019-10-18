function T = vpt(X, idx)

    n = size(X,1); % number of points

    if n == 0

        T = [];

    else

        T.vp = X(n,:);
        T.idx = idx(n);

        d = sqrt( sum((X(1:n-1,:) - X(n,:)).^2,2) )

        medianDistance = median(d);
        T.md = medianDistance;

        % split and recurse
        inner = d <= medianDistance;

        X_inner = X( inner,:);
        X_outer = X( ~inner,:);

        X_inner = X_inner(:)
        X_outer = X_outer(:)

        T.inner = vpt(X( inner,:), idx( inner));
        T.outer = vpt(X(~inner,:), idx(~inner));

    end

end